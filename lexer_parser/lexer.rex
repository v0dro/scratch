class AddLexer
macro
  DIGIT         /\d+/
rule
  /#{DIGIT}/    { [:DIGIT, text.to_i] }
  /.|\n/        { [text, text] }
inner
  def do_parse; end # this is a stub.
end # AddLexer