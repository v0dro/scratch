# encoding: UTF-8
#--
# This file is automatically generated. Do not modify it.
# Generated by: oedipus_lex version 2.4.1.
# Source: lexer.rex
#++

class AddLexer
  require 'strscan'

  DIGIT = /\d+/

  class ScanError < StandardError ; end

  attr_accessor :filename
  attr_accessor :ss
  attr_accessor :state

  alias :match :ss

  def matches
    m = (1..9).map { |i| ss[i] }
    m.pop until m[-1] or m.empty?
    m
  end

  def action
    yield
  end


  def scanner_class
    StringScanner
  end unless instance_methods(false).map(&:to_s).include?("scanner_class")

  def parse str
    self.ss     = scanner_class.new str
    self.state  ||= nil

    do_parse
  end

  def parse_file path
    self.filename = path
    open path do |f|
      parse f.read
    end
  end

  def next_token

    token = nil

    until ss.eos? or token do
      token =
        case state
        when nil then
          case
          when text = ss.scan(/#{DIGIT}/) then
            action { [:DIGIT, text.to_i] }
          when text = ss.scan(/.|\n/) then
            action { [text, text] }
          else
            text = ss.string[ss.pos .. -1]
            raise ScanError, "can not match (#{state.inspect}): '#{text}'"
          end
        else
          raise ScanError, "undefined state: '#{state}'"
        end # token = case state

      next unless token # allow functions to trigger redo w/ nil
    end # while

    raise "bad lexical result: #{token.inspect}" unless
      token.nil? || (Array === token && token.size >= 2)

    # auto-switch state
    self.state = token.last if token && token.first == :state

    token
  end # def next_token
    def do_parse; end # this is a stub.
end # class

   # AddLexer