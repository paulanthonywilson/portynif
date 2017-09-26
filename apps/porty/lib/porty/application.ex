defmodule Porty.Application do
  @moduledoc false

  use Application

  def start(_type, _args) do
    children = [
      {Registry, keys: :duplicate, name: :event_registry},
      Porty.Echo,
    ]

    opts = [strategy: :one_for_one, name: Porty.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
