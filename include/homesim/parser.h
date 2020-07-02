/**
 * \file homesim/parser.h
 *
 * \brief Lexer and parser declarations for parsing homesim files or running the
 * homesim REPL.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_PARSER_HEADER_GUARD
# define HOMESIM_PARSER_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace homesim {

/**
 * \brief Tokens supported by the lexer.
 */
enum token
{
    /* assignment. */
    HOMESIM_TOKEN_ASSIGN,
    /* asterisk. */
    HOMESIM_TOKEN_ASTERISK,
    /* left brace. */
    HOMESIM_TOKEN_BRACE_LEFT,
    /* right brace. */
    HOMESIM_TOKEN_BRACE_RIGHT,
    /* left bracket. */
    HOMESIM_TOKEN_BRACKET_LEFT,
    /* right bracket. */
    HOMESIM_TOKEN_BRACKET_RIGHT,
    /* dot. */
    HOMESIM_TOKEN_DOT,
    /* equals. */
    HOMESIM_TOKEN_EQUALS,
    /* identifier. */
    HOMESIM_TOKEN_IDENTIFIER,
    /* after keyword. */
    HOMESIM_TOKEN_KEYWORD_AFTER,
    /* assert keyword. */
    HOMESIM_TOKEN_KEYWORD_ASSERT,
    /* at keyword. */
    HOMESIM_TOKEN_KEYWORD_AT,
    /* component keyword. */
    HOMESIM_TOKEN_KEYWORD_COMPONENT,
    /* examine keyword. */
    HOMESIM_TOKEN_KEYWORD_EXAMINE,
    /* execution keyword. */
    HOMESIM_TOKEN_KEYWORD_EXECUTION,
    /* expect keyword. */
    HOMESIM_TOKEN_KEYWORD_EXPECT,
    /* export keyword. */
    HOMESIM_TOKEN_KEYWORD_EXPORT,
    /* external keyword. */
    HOMESIM_TOKEN_KEYWORD_EXTERNAL,
    /* false keyword. */
    HOMESIM_TOKEN_KEYWORD_FALSE,
    /* module keyword. */
    HOMESIM_TOKEN_KEYWORD_MODULE,
    /* pin keyword. */
    HOMESIM_TOKEN_KEYWORD_PIN,
    /* probe keyword. */
    HOMESIM_TOKEN_KEYWORD_PROBE,
    /* pulldown keyword. */
    HOMESIM_TOKEN_KEYWORD_PULLDOWN,
    /* pullup keyword. */
    HOMESIM_TOKEN_KEYWORD_PULLUP,
    /* scenario keyword. */
    HOMESIM_TOKEN_KEYWORD_SCENARIO,
    /* signal keyword. */
    HOMESIM_TOKEN_KEYWORD_SIGNAL,
    /* source keyword. */
    HOMESIM_TOKEN_KEYWORD_SOURCE,
    /* start keyword. */
    HOMESIM_TOKEN_KEYWORD_START,
    /* state keyword. */
    HOMESIM_TOKEN_KEYWORD_STATE,
    /* true keyword. */
    HOMESIM_TOKEN_KEYWORD_TRUE,
    /* type keyword. */
    HOMESIM_TOKEN_KEYWORD_TYPE,
    /* wire keyword. */
    HOMESIM_TOKEN_KEYWORD_WIRE,
    /* decimal number (double). */
    HOMESIM_TOKEN_NUMBER,
    /* left paren. */
    HOMESIM_TOKEN_PAREN_LEFT,
    /* right paren. */
    HOMESIM_TOKEN_PAREN_RIGHT,
    /* quoted string. */
    HOMESIM_TOKEN_STRING,

    /* end of input. */
    HOMESIM_TOKEN_EOF,

    /* an invalid token. */
    HOMESIM_TOKEN_INVALID
};

typedef std::pair<token, std::string>
token_pair;

/**
 * Simple utility function to transform a lexer token into a string description.
 *
 * \param t         The token to convert to a string.
 *
 * \returns a string description of the token.
 */
std::string token_to_description(token t);

/**
 * \brief The lexer tokenizes an input stream.  These tokens can then be passed
 * to a parser instance to parse a homesim stream.
 */
class lexer
{
public:

    /**
     * \brief Create a lexer instance backed by the given input stream.
     *
     * \param input         The input stream for this instance.
     */
    lexer(std::istream& input);

    /**
     * \brief Read a token from the stream.
     *
     * \returns the token read.
     *      - HOMESIM_TOKEN_INVALID if invalid data was read from the stream.
     *      - HOMESIM_TOKEN_EOF if at the end of the stream.
     */
    token read();

    /**
     * \brief Get the line and column information for the current token.
     *
     * \param start_line        This reference is set to the start line of the
     *                          token.
     * \param start_col         This reference is set to the start column of the
     *                          token.
     * \param end_line          This reference is set to the end line of the
     *                          token.
     * \param end_col           This reference is set to the end column of the
     *                          token.
     */
    void read_linecol(
        int& start_line, int& start_col, int& end_line, int& end_col) const;

    /**
     * \brief Get the string value of the current token.
     *
     * \returns string value of the token or an empty string if inappropriate.
     */
    std::string get_token_string();

private:
    std::istream& in;
    std::list<char> tokenbuf;
    std::list<int> putbackbuf;
    int curline;
    int curcol;
    int start_line;
    int start_col;
    int end_line;
    int end_col;

    int read_char();
    void start(int ch);
    void accept(int ch);
    void put_back(int ch);

    token matchSequence(
        const std::string& seq, std::function<token ()> onAccept,
        std::function<token ()> onFail);

    token matchKeywordOrIdentifier(
        const std::string& seq, token keywordToken);

    token matchEndOfKeyword(
        std::function<token ()> onAccept,
        std::function<token ()> onFail);

    token maybeReadAssign();
    token maybeReadAsterisk();
    token maybeReadDotOrNumber();
    token maybeReadNumberWithAtLeastOneDigit();
    token maybeReadNumber();
    token maybeReadDecimalNumber();
    token maybeReadExponentNumberWithSign();
    token maybeReadExponentNumber();
    token maybeReadKeywordAfterAssertAtOrIdentifier();
    token maybeReadKeywordExamineExecutionExpectExportExternalOrIdentifier();
    token maybeReadKeywordExpectExportOrIdentifier();
    token maybeReadKeywordPinProbePulldownPullupOrIdentifier();
    token maybeReadKeywordPulldownPullupOrIdentifier();
    token maybeReadKeywordPullupOrIdentifier();
    token maybeReadKeywordScenarioSignalStartStateOrIdentifier();
    token maybeReadKeywordStartStateOrIdentifier();
    token maybeReadKeywordTrueTypeOrIdentifier();
    token maybeReadIdentifier();
    token maybeReadStartIdentifier();
    token maybeReadString();
};

struct config_ast_module;
struct config_ast_component;
struct config_ast_connection;
struct config_ast_wire;
struct config_ast_probe;
struct config_ast_scenario;
struct config_ast_expression;
struct config_ast_execution;
struct config_ast_step;
struct config_ast_assertion;

struct config_ast_module
{
    std::string name;
    std::multimap<std::string, std::shared_ptr<config_ast_component>>
    component_map;
    std::multimap<std::string, std::shared_ptr<config_ast_wire>> wire_map;
    std::map<std::string, std::shared_ptr<config_ast_probe>> probe_map;
    std::map<std::string, std::shared_ptr<config_ast_scenario>> scenario_map;
};

struct config_ast_component
{
    std::string name;
    std::shared_ptr<std::string> type;
    std::map<std::string, std::shared_ptr<config_ast_expression>> config_map;
};

struct config_ast_wire
{
    std::string name;
    bool exported;
    bool external_source;
    std::list<std::shared_ptr<config_ast_connection>> connection_list;
    std::shared_ptr<config_ast_component> pullup_pulldown;
};

struct config_ast_connection
{
    std::string component;
    std::string pin;
};

struct config_ast_scenario
{
    std::string name;
    std::map<std::string, std::shared_ptr<config_ast_execution>> execution_map;
};

struct config_ast_execution
{
    std::string name;
    std::list<std::shared_ptr<config_ast_step>> step_list;
};

struct config_ast_expression
{
    virtual ~config_ast_expression() { }
    virtual token type() = 0;
    virtual std::string eval() = 0;
};

struct config_ast_simple_expression : public config_ast_expression
{
    virtual token type() { return ty; }
    virtual std::string eval() { return simple_value; }

    std::string simple_value;
    token ty;
};

struct config_ast_complex_expression : public config_ast_expression
{
    virtual token type();
    virtual std::string eval();

    std::string functor;
    std::list<token_pair> args;
};

struct config_ast_assignment
{
    std::string lhs_major;
    std::string lhs_minor;
    std::shared_ptr<config_ast_expression> rhs;
};

struct config_ast_probe
{
    std::string name;
    std::string type;
    std::shared_ptr<std::string> sub_type;
    std::list<std::string> wire_ref_list;
};

struct config_ast_step
{
    std::string type;
    std::shared_ptr<config_ast_expression> step_expression;
    std::list<std::shared_ptr<config_ast_assignment>> pin_assignments;
    std::list<std::shared_ptr<config_ast_assertion>> assertion_list;
};

struct config_ast_assertion
{
    std::string type;
    std::list<std::string> lhs;
    std::shared_ptr<config_ast_expression> rhs;
};

/**
 * \brief This exception is thrown when the parser fails to parse a stream.
 */
class parser_error : public std::runtime_error
{
public:
    parser_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

/**
 * \brief The parser parses an input stream into expressions that can be
 * evaluated.
 */
class parser
{
public:

    /**
     * \brief Create a parser instance backed by the given input stream.
     *
     * \param input         The input stream for this instance.
     */
    parser(std::istream& input);

    /**
     * \brief Parse the input stream producing a config ast.
     *
     * \returns the module AST on success.
     *
     * \throws a parser_error on failure.
     */
    std::shared_ptr<config_ast_module> parse();

private:
    lexer in;
    std::list<token_pair> lookahead;

    token_pair read();
    void put_back(const token_pair&);
    std::shared_ptr<config_ast_component> parse_component();
    std::shared_ptr<config_ast_component> parse_pull(const std::string& type);
    std::shared_ptr<config_ast_wire> parse_wire();
    void parse_wire_signal_source();
    std::shared_ptr<config_ast_wire> parse_export_wire();
    std::shared_ptr<config_ast_probe> parse_probe(const std::string& type);
    std::shared_ptr<config_ast_assignment> parse_assign(const std::string& id);
    std::shared_ptr<config_ast_assignment>
    parse_pin_assign(const std::string& id);
    std::shared_ptr<config_ast_connection>
    parse_connection(const std::string& id);
    std::shared_ptr<config_ast_scenario> parse_scenario();
    std::shared_ptr<config_ast_execution> parse_execution();
    std::shared_ptr<config_ast_step> parse_step(const std::string& type);
    std::shared_ptr<config_ast_assertion>
    parse_assertion(const std::string& type);
    std::shared_ptr<std::function<void(const token_pair&)>>
    simple_assignment_maker(
        std::shared_ptr<config_ast_assignment>);
    void handle_assignment(
        std::shared_ptr<config_ast_component> component,
        std::shared_ptr<config_ast_assignment> assignment);
    std::shared_ptr<std::string> parse_type();
    std::shared_ptr<std::string> parse_wire_ref();
    std::shared_ptr<config_ast_expression>
    parse_complex_expression(const std::string&);
    std::shared_ptr<config_ast_expression>
    parse_inner_expression(const std::string&);

    void
    parse_sequence(
        std::initializer_list<
            std::pair<
                token,
                std::shared_ptr<std::function<void (const token_pair&)>>>>
            seq);

    void
    parse_choose(
        std::initializer_list<
            std::pair<
                token,
                std::shared_ptr<std::function<void (const token_pair&)>>>>
            choices);

    std::string trim_string(const std::string& str);
};

} /* namespace homesim */

#endif /*HOMESIM_PARSER_HEADER_GUARD*/
