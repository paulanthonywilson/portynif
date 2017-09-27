defmodule Porty.Echo do
  @moduledoc """
  Uses a port to echo back messages to subscribed processes.
  """

  use GenServer

  @name __MODULE__

  def start_link(_) do
    GenServer.start_link(__MODULE__, {}, name: @name)
  end

  def init(_) do
    port = Port.open({:spawn, :code.priv_dir(:porty) ++ '/c/echo'}, [{:packet, 2}])
    {:ok, port}
  end

  @doc """
  Process receives echoes with the signature
  `{:echo, String.t}`
  """
  @spec subscribe :: {:ok, pid} | {:error, {:already_registered, pid}}
  def subscribe do
    Registry.register(:event_registry, :echo, [])
  end

  @doc """
  Send the msg to the port. It is echoed back, turned into a binary, and sent
  to any subscribers.
  """
  @spec say(String.t) :: :ok
  def say(msg) do
    GenServer.cast(@name, {:say, msg})
    :ok
  end

  def handle_cast({:say, msg}, port) do
    Port.command(port, msg)
    {:noreply, port}
  end

  def handle_info({_, {:data, msg}}, port) do
    msg
    |> List.to_string()
    |> broadcast()
    {:noreply, port}
  end

  defp broadcast(msg) do
    Registry.dispatch(:event_registry, :echo, fn entries ->
      for {pid, _} <- entries, do: send(pid, {:echo, msg})
    end)
  end
end
