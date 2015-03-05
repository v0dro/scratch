require 'pi_piper'
require 'rubygame'

include PiPiper

sound1 = Rubygame::Sound.load("r.wav")
sound2 = Rubygame::Sound.load("w.wav")

sounds = [sound1, sound2].cycle.each
current = nil

watch :pin => 4, :direction => :in, :pull => :down, :trigger => :both do 
  current.pause if current
  current = sounds.next
  current.play :repeats => -1
end

#watch :pin => 4, :direction => :in, :pull => :down, :trigger => :falling do 
#  sounds.next.play :repeats => -1
#end

wait
