defmodule PortyTest do
  use ExUnit.Case
  doctest Porty

  test "greets the world" do
    assert Porty.hello() == :world
  end
end
