defmodule Porty.Echo do
  @moduledoc """
  Uses a port to echoe back messages to subscribed processes.
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

  def subscribe do
    Registry.register(:event_registry, :echo, [])
  end

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
