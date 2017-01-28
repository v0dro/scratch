
require_relative 'blackjack'

def test_stupid_player
  game = BlackJack::Game.new
  game.shuffle_cards!
  game.deal_cards

  until game.over?
    puts "#{game.current_player}"
    game.action :hit
  end

  game.result
end

def test_smart_player
  game = BlackJack::Game.new
  game.shuffle_cards!
  game.deal_cards

  until game.over?
    game.action :hit
    if game.dealer.score > 17
      game.action :stand
    else
      game.action :hit
    end
  end

  game.result
end

puts "win #{test_stupid_player}"
test_smart_player
