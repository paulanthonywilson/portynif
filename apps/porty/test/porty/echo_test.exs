defmodule Porty.EchoTest do
  use ExUnit.Case

  alias Porty.Echo

  setup do
    Echo.subscribe
    :ok
  end

  test "echoes" do
    Echo.say("My heart aches and a drowsy numbness pains my sense")
    assert_receive {:echo, "My heart aches and a drowsy numbness pains my sense"}
  end
end
