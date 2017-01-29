
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
    game.action :stand
  end

  game.result
end

def test_game_proper
  game = BlackJack::Game.new
  game.shuffle_cards!
  game.deal_cards

  if game.over?
    puts "The winner is : #{game.result}"
    return
  end

  until game.over?
    puts "Player action(hit/stand)>"
    move = STDIN.gets.chomp.to_sym
    begin
      game.action move
    rescue ArgumentError
      puts "Only permissible actions are hit and stand."
    end
  end

  puts "The winner is: #{game.result}"
  puts "Scores:"
  puts "Player: #{game.player.score}"
  puts "Dealer: #{game.dealer.score}"
  puts "Cards: "
  puts "Player: #{game.player.cards}"
  puts "Dealer: #{game.dealer.cards}"
end

test_stupid_player
test_smart_player
test_game_proper if ARGV[0] == 'play'
