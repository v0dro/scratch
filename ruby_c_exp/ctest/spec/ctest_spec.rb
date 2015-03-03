require 'ctest'

describe CTest do
  it "writes my first ext" do
    expect(CTest.hello_world).to eq('hello world!')
  end
end