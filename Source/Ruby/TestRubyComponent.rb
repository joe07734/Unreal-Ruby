class TestRubyComponent
  def beginPlay
    @count = 0
  end

  def endPlay
  end

  def tickComponent
    @count += 1
    if @count <= 30 || @count % 60 == 0
      puts @count
    end
  end
end

puts "TestRubyComponent.rb loaded"
