#ifndef MYMAKE_PARSER_H_
#define MYMAKE_PARSER_H_

namespace mymake {

class Parser {

public:
  Parser() {};
  virtual ~Parser() {};
  virtual bool Driver( Lexer *lexer );

private:
};
} // end namespace mymake

#endif // MYMAKE_PARSER_H_

