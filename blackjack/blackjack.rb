module BlackJack
  SCORES = {
    'J' => 10,
    'Q' => 10,
    'K' => 10,
    'A' => [1,11],
    '2' => 2,
    '3' => 3,
    '4' => 4,
    '5' => 5,
    '6' => 6,
    '7' => 7,
    '8' => 8,
    '9' => 9,
    '10' => 10
  }

  class Game
    attr_reader :current_player
    attr_reader :result, :player, :dealer

    def initialize decks=4
      decks < 4 and raise("Must have 4 or more decks.")
      @deck = Deck.new decks
      @result = nil
      @over = false
    end

    def shuffle_cards!
      @deck.shuffle!
    end

    def deal_cards
      @player = Player.new @deck
      @dealer = Dealer.new @deck
      @current_player = @player
      compute_score_for @player, first_move: true
      compute_score_for @dealer, first_move: true
      @result = check_blackjack
      @over = true if @result
    end

    def over?
      @over
    end

    def action act
      @player.send(act, @deck)
      compute_score_for @player
      change_current_player if act == :stand
      hits_for_dealer if @curent_player == @dealer && @dealer.score <= 17

      @result = check_winner_tie_loser
      if !@result.nil?
        @over = true
        return
      end
    end

  private

    def check_blackjack
      if @player.score == 21 && @dealer.score == 21
        :tie
      elsif @player.score == 21
        :dealer
      elsif @dealer.score == 21
        :player
      else
        nil
      end
    end

    def hits_for_dealer
      while @dealer.score <= 17
        @dealer.get_card
        compute_score_for @dealer
      end
    end

    def check_winner_tie_loser
      if @player.score > 21
        :dealer
      elsif @dealer.score > 21
        :player
      elsif @player.score == @dealer.score
        :tie
      elsif @dealer.score > 17
        if @player.score > @dealer.score
          :player
        elsif @dealer.score > @player.score
          :dealer
        end
      end
    end

    def change_current_player
      @current_player = @dealer if @current_player == @player
      @current_player = @player if @current_player == @dealer
    end

    def compute_score_for participant, first_move: false
      cards_without_a = participant.cards.select { |c| c != 'A' }
      score = cards_without_a.inject(0) { |s, c| s += SCORES[c]; s }
      a_cards = participant.cards - cards_without_a
      if first_move && !a_cards.empty?
        a_cards.size.times { score += 11 }
      elsif !a_cards.empty?
        a_cards.size.times { score += 1 }
      end
      participant.score = score
    end
  end

  class Participant
    attr_reader :cards
    attr_accessor :score

    def initialize deck
      @score = 0
      @cards = 2.times.map { deck.get_card }
    end

    def hit deck
      @cards << deck.get_card
    end

    def stand deck

    end

    def == other
      self.class == other.class
    end
  end

  class Player < Participant
    def to_s
      "player"
    end
  end

  class Dealer < Participant
    def to_s
      "dealer"
    end
  end

  class Deck
    SUITS = 4
    FACES = ['A', *(2..10).map(&:to_s), 'J', 'Q', 'K']

    def initialize decks
      create_cards decks
    end

    def shuffle!
      @cards.shuffle!
    end

    def get_card
      @cards.shift
    end

  private

    def create_cards decks
      @cards = FACES*SUITS*decks
    end
  end
end
