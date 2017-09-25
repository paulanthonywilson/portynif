defmodule Porty.Mixfile do
  use Mix.Project

  def project do
    [
      app: :porty,
      version: "0.1.0",
      build_path: "../../_build",
      config_path: "../../config/config.exs",
      deps_path: "../../deps",
      lockfile: "../../mix.lock",
      compilers: [:elixir_make | Mix.compilers],
      elixir: "~> 1.5",
      start_permanent: Mix.env == :prod,
      deps: deps()
    ]
  end

  def application do
    [
      extra_applications: [:logger],
      mod: {Porty.Application, []}
    ]
  end

  defp deps do
    [
      {:elixir_make, "~> 0.4"},
    ]
  end
end
