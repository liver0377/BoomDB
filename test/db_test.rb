describe 'database' do
  def run_script(commands)                 # run_script()用于运行脚本
    raw_output = nil
    IO.popen("./db", "r+") do |pipe|       # 使用pipe接受参数
      commands.each do |command|           # 同上, .each方法表示循环
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'test exit and unrecognized command' do
    result = run_script([
      "hello world",
      "HELLO WORLD",
      ".exit",
    ])
    expect(result).to match_array([
      "Boom > unrecognized command: hello world",
      "Boom > unrecognized command: HELLO WORLD",
      "Boom > Bye!",
    ])
  end
end