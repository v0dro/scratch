#
# DO NOT MODIFY!!!!
# This file is automatically generated by Racc 1.4.14
# from Racc grammer file "".
#

require 'racc/parser.rb'

require_relative 'lexer.rex.rb'

class AddParser < Racc::Parser

module_eval(<<'...end parser.racc/module_eval...', 'parser.racc', 13)
def next_token
  @lexer.next_token
end

def prepare_parser file_name
  @lexer = AddLexer.new
  @lexer.parse_file file_name
end
...end parser.racc/module_eval...
##### State transition tables begin ###

racc_action_table = [
     3,     4,     5,     6,     3,     5 ]

racc_action_check = [
     0,     1,     2,     4,     5,     7 ]

racc_action_pointer = [
    -3,     1,     0,   nil,     3,     1,   nil,     3 ]

racc_action_default = [
    -4,    -4,    -1,    -3,    -4,    -4,     8,    -2 ]

racc_goto_table = [
     2,     1,   nil,   nil,   nil,     7 ]

racc_goto_check = [
     2,     1,   nil,   nil,   nil,     2 ]

racc_goto_pointer = [
   nil,     1,     0 ]

racc_goto_default = [
   nil,   nil,   nil ]

racc_reduce_table = [
  0, 0, :racc_error,
  1, 5, :_reduce_1,
  3, 6, :_reduce_2,
  1, 6, :_reduce_none ]

racc_reduce_n = 4

racc_shift_n = 8

racc_token_table = {
  false => 0,
  :error => 1,
  "+" => 2,
  :DIGIT => 3 }

racc_nt_base = 4

racc_use_result_var = true

Racc_arg = [
  racc_action_table,
  racc_action_check,
  racc_action_default,
  racc_action_pointer,
  racc_goto_table,
  racc_goto_check,
  racc_goto_default,
  racc_goto_pointer,
  racc_nt_base,
  racc_reduce_table,
  racc_token_table,
  racc_shift_n,
  racc_reduce_n,
  racc_use_result_var ]

Racc_token_to_s_table = [
  "$end",
  "error",
  "\"+\"",
  "DIGIT",
  "$start",
  "target",
  "exp" ]

Racc_debug_parser = false

##### State transition tables end #####

# reduce 0 omitted

module_eval(<<'.,.,', 'parser.racc', 2)
  def _reduce_1(val, _values, result)
     result = 0 
    result
  end
.,.,

module_eval(<<'.,.,', 'parser.racc', 4)
  def _reduce_2(val, _values, result)
     result += val[2]; puts result 
    result
  end
.,.,

# reduce 3 omitted

def _reduce_none(val, _values, result)
  val[0]
end

end   # class AddParser
