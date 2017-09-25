defmodule Porty do
  @moduledoc """
  Documentation for Porty.
  """

  @doc """
  Hello world.

  ## Examples

      iex> Porty.hello
      :world

  """
  def hello do
    __DIR__
  end

  def open_port do
    Process.flag(:trap_exit, true)
    Port.open({:spawn, :code.priv_dir(:porty) ++ '/c/echo'}, [{:packet, 2}])
  end
end
