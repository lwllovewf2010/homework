#ifndef MYMAKE_LEXER_H_
#define MYMAKE_LEXER_H_

#define LEXER_TMPBUF_SIZE 2048

namespace mymake {

class Lexer {

public:
  enum Token {
    TOK_INVALID = 0,
    TOK_NEWLINE,
    TOK_COMMENT,
    TOK_SPACE,
    TOK_STR,
    TOK_TAB,
    TOK_COLON,
    TOK_QMARK,
    TOK_EQUAL,
    TOK_DALLOR,
    TOK_PLUS,
    TOK_AT,
    TOK_LESSER,
    TOK_GREATER,
    TOK_VAR,
    TOK_PRECENT,
    TOK_CARET,
    TOK_STAR,
    TOK_BOF,
    TOK_EOF,

    TOK_VPATH,
    TOK_VPATH_CAPS,
    TOK_INCLUDE,
    TOK_IF,
    TOK_IFDEF,
    TOK_IFNDEF,
    TOK_ENDEF,
    TOK_IFEQ,
    TOK_IFNEQ,
    TOK_ELSE,
    TOK_ENDIF,

    TOK_LOAD,
    TOK_DEF_GOAL,
    TOK_DEF,
    TOK_DEL_ON_ERR,
    TOK_EXP_ALL_VAR,
    TOK_FEATURES,
    TOK_IGNORE,
    TOK_INC_DIRS,
    TOK_INTERMED,
    TOK_LIBPATTERNS,
    TOK_LOADED,
    TOK_LOW_RES_TIME,
    TOK_NOTPARALLEL,
    TOK_ONESHELL,
    TOK_PHONY,
    TOK_POSIX,
    TOK_PRECIOUS,
    TOK_RECIPE_PREFIX,
    TOK_SECONDARY,
    TOK_SECONDEXPANSION,
    TOK_SHELLFLAGS,
    TOK_SLIENT,
    TOK_SUFFIXES,
    TOK_VARIABLES,
  };

  enum LineType {
    LP_NORMAL = 0,
    LP_TARGET,
    LP_RECEIPT,
    LP_VARIABLE,
    LP_AMB_REC_VAR,
  };
  
  typedef struct _charToToken {
    u8 c;
    Token tok;
  } charToToken_t;
  
  typedef struct _keywordToToken {
    const s8 *keyStr;
    u8 keyStrLen;
    Token tok;
  } keywordToToken_t;

public:
  Lexer() { ClearStates(); }
  virtual ~Lexer() {};

  virtual void Restart();
  virtual bool Open( const s8 *path );
  virtual bool Open( const std::string &path );
  virtual void Close();
  virtual bool IsInit() { return initialized; }
  virtual bool IsBOF() { return (c == fp) ? true : false; }
  virtual bool IsEOF() { return ((c - fp) >= fsize) ? true : false; }
  virtual bool IsBOL();

  virtual Token GetNextToken();
  virtual Token LookAheadToken();

  virtual bool ToNextLine();
  virtual bool ToPrevLine();
	virtual bool ToBeginOfLine();

  virtual LineType PredictLineType();

  virtual const u8 *ExtractTokenStr();
  virtual const u8 *ExtractLineStr();
  
  virtual const u8 *GetTokenStr() const
    { return tmpBuf; }
  virtual const s32 GetTokenStrLen() const
    { return tmpBufLen; }
	virtual const u32 GetLineNum() const
    { return currLine; };
	virtual const u32 GetLineOffStart() const
    { return (currTok == TOK_NEWLINE) ? 0 : (tokenStrStart - lineStartP); }
	virtual const u32 GetLineOffEnd() const
    { return (currTok == TOK_NEWLINE) ? 0 : (tokenStrEnd - lineStartP); }

private:
  // File handle
  s32 ifd;
  s32 fsize, restLen;
  u8 *fp, *c;
  bool initialized;

  // Buffer of token string
  u8 tmpBuf[ LEXER_TMPBUF_SIZE ];
  s32 tmpBufLen;

  // Token
	Token currTok, nextTok;

  // Token string
  u8 *tokenStrStart, *nextTokenStrStart;
  u8 *tokenStrEnd, *nextTokenStrEnd;
  
  // Line
  u8 *lineStartP;
  u32 currLine;

  // State backup
  s32 bakLen;
  u32 bakLine;
  u8 *bakC;
  Token bakTok;
  u8 *bakTokenStrStart, *bakTokenStrEnd, *bakLineStartP;

private:
  virtual void ClearStates();
  virtual void SaveStates();
  virtual void RestoreStates();
};
} // end namespace mymake

#endif // MYMAKE_LEXER_H_

