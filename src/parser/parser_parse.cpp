/**
 * \file parser/parser_parse.cpp
 *
 * \brief Parse the input stream.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/parser.h>

using namespace homesim;
using namespace std;

/* macro hackery to make the parser below a bit easier to understand. */
#define SEQUENCE_BEGIN() \
    parse_sequence({
#define SEQUENCE_END() \
    });
#define SEQ(tok) \
    make_pair(tok, nullptr)
#define SEQL_BEGIN(tok, var) \
    make_pair(tok, \
    make_shared<function<void(const token_pair&)>>( \
        [&](const token_pair& var) {
#define SEQL_END() \
    }))
#define CHOOSE_BEGIN() \
    parse_choose({
#define CHOOSE_END() \
    });
#define OPTION(token) SEQ(token)
#define OPTIONL_BEGIN(token,var) SEQL_BEGIN(token,var)
#define OPTIONL_END() SEQL_END()
#define OPTIONF(tok, func) \
    make_pair(tok, func)

/**
 * \brief Parse the input stream producing a config ast.
 *
 * \returns a pair that contains a shared pointer to the config_ast_module
 * and a shared pointer to a list of error strings.  On success, the config
 * module will be populated and the error list pointer will not. On failure,
 * vice versa.
 */
shared_ptr<config_ast_module> homesim::parser::parse()
{
    auto module = make_shared<config_ast_module>();

    /* attempt to parse "module identifier {" */
    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_KEYWORD_MODULE),
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            module->name = t.second;
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT,t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_COMPONENT,t)
                auto component = parse_component();
                module->component_map.insert(
                    make_pair(component->name, component));
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_EXPORT,t)
                auto wire = parse_export_wire();
                module->wire_map.insert(make_pair(wire->name, wire));
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_WIRE,t)
                auto wire = parse_wire();
                module->wire_map.insert(make_pair(wire->name, wire));
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_SIGNAL,t)
                auto probe = parse_probe(t.second);
                module->probe_map.insert(make_pair(probe->name, probe));
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_STATE,t)
                auto probe = parse_probe(t.second);
                module->probe_map.insert(make_pair(probe->name, probe));
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_SCENARIO,t)
                auto scenario = parse_scenario();
                module->scenario_map.insert(
                    make_pair(scenario->name, scenario));
            OPTIONL_END()
        CHOOSE_END()
    }

    /* success. */
    return module;
}

shared_ptr<config_ast_component> homesim::parser::parse_component()
{
    auto component = make_shared<config_ast_component>();

    SEQUENCE_BEGIN()
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            component->name = t.second;
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto assign = parse_assign(t.second);
                handle_assignment(component, assign);
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_TYPE, t)
                auto type = parse_type();
                component->type = type;
            OPTIONL_END()
        CHOOSE_END()
    }

    return component;
}

shared_ptr<config_ast_wire> homesim::parser::parse_export_wire()
{
    shared_ptr<config_ast_wire> wire;

    SEQUENCE_BEGIN()
        SEQL_BEGIN(HOMESIM_TOKEN_KEYWORD_WIRE, t)
            wire = parse_wire();
            wire->exported = true;
        SEQL_END(),
    SEQUENCE_END()

    return wire;
}

shared_ptr<config_ast_wire> homesim::parser::parse_wire()
{
    auto wire = make_shared<config_ast_wire>();

    SEQUENCE_BEGIN()
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            wire->name = t.second;
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto connection = parse_connection(t.second);
                wire->connection_list.push_back(connection);
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_PULLUP, t)
                auto pullup = parse_pull(t.second);
                wire->pullup_pulldown = pullup;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_PULLDOWN, t)
                auto pulldown = parse_pull(t.second);
                wire->pullup_pulldown = pulldown;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_SIGNAL, t)
                parse_wire_signal_source();
                wire->external_source = true;
            OPTIONL_END()
        CHOOSE_END()
    }

    return wire;
}

void homesim::parser::parse_wire_signal_source()
{
    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_KEYWORD_SOURCE),
        SEQ(HOMESIM_TOKEN_KEYWORD_EXTERNAL)
    SEQUENCE_END()
}

shared_ptr<config_ast_probe>
homesim::parser::parse_probe(const string& type)
{
    auto probe = make_shared<config_ast_probe>();
    probe->type = type;

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_KEYWORD_PROBE),
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            probe->name = t.second;
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_TYPE, t)
                auto type = parse_type();
                probe->sub_type = type;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_WIRE, t)
                auto wire_ref = parse_wire_ref();
                probe->wire_ref_list.push_back(*wire_ref);
            OPTIONL_END()
        CHOOSE_END()
    }

    return probe;
}

shared_ptr<string> homesim::parser::parse_type()
{
    string name;

    SEQUENCE_BEGIN()
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            name = t.second;
        SEQL_END()
    SEQUENCE_END()

    return make_shared<string>(name);
}

shared_ptr<string> homesim::parser::parse_wire_ref()
{
    string ref;

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            ref = t.second;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_ASTERISK, t)
            ref = t.second;
        OPTIONL_END()
    CHOOSE_END()

    return make_shared<string>(ref);
}

shared_ptr<config_ast_assignment>
homesim::parser::parse_assign(
    const string& id)
{
    auto assignment = make_shared<config_ast_assignment>();
    assignment->lhs_major = id;

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_BRACKET_LEFT),
        SEQL_BEGIN(HOMESIM_TOKEN_STRING, t)
            assignment->lhs_minor =
                trim_string(t.second);
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACKET_RIGHT),
        SEQ(HOMESIM_TOKEN_ASSIGN)
    SEQUENCE_END()

    CHOOSE_BEGIN()
        OPTIONF(HOMESIM_TOKEN_NUMBER, simple_assignment_maker(assignment)),
        OPTIONF(HOMESIM_TOKEN_STRING, simple_assignment_maker(assignment)),
        OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            auto expression = parse_complex_expression(t.second);
            assignment->rhs = expression;
        OPTIONL_END()
    CHOOSE_END()

    return assignment;
}

shared_ptr<config_ast_assignment>
homesim::parser::parse_pin_assign(
    const string& id)
{
    auto assignment = make_shared<config_ast_assignment>();
    assignment->lhs_major = id;

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_DOT),
        SEQ(HOMESIM_TOKEN_KEYWORD_PIN),
        SEQ(HOMESIM_TOKEN_BRACKET_LEFT),
        SEQL_BEGIN(HOMESIM_TOKEN_STRING, t)
            assignment->lhs_minor =
                trim_string(t.second);
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACKET_RIGHT),
        SEQ(HOMESIM_TOKEN_ASSIGN)
    SEQUENCE_END()

    CHOOSE_BEGIN()
        OPTIONF(
            HOMESIM_TOKEN_KEYWORD_TRUE,
            simple_assignment_maker(assignment)),
        OPTIONF(
            HOMESIM_TOKEN_KEYWORD_FALSE,
            simple_assignment_maker(assignment)),
    CHOOSE_END()

    return assignment;
}

void homesim::parser::handle_assignment(
    shared_ptr<config_ast_component> component,
    shared_ptr<config_ast_assignment> assignment)
{
    if ("config" == assignment->lhs_major)
    {
        component->config_map.insert(
            make_pair(assignment->lhs_minor, assignment->rhs));
    }
    /* TODO - add warning if a different assignment lhs is used. */
}

shared_ptr<config_ast_connection>
homesim::parser::parse_connection(const string& id)
{
    auto connection = make_shared<config_ast_connection>();
    connection->component = id;

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_DOT),
        SEQ(HOMESIM_TOKEN_KEYWORD_PIN),
        SEQ(HOMESIM_TOKEN_BRACKET_LEFT),
        SEQL_BEGIN(HOMESIM_TOKEN_STRING, t)
            connection->pin =
                trim_string(t.second);
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACKET_RIGHT)
    SEQUENCE_END()

    return connection;
}

string homesim::parser::trim_string(const string& str)
{
    return string(str.begin()+1, str.end()-1);
}

shared_ptr<function<void(const token_pair&)>>
homesim::parser::simple_assignment_maker(
    shared_ptr<config_ast_assignment> assignment)
{
    return
        make_shared<function<void(const token_pair&)>>(
            [=](const token_pair& t) {
                auto assign =
                    make_shared<config_ast_simple_expression>();
                assign->simple_value = t.second;
                assign->ty = t.first;
                assignment->rhs = assign;
            });
}

shared_ptr<config_ast_expression>
homesim::parser::parse_complex_expression(const string& id)
{
    shared_ptr<config_ast_expression> expr;

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_LEFT, t)
            /* we're looking ahead, so save the token. */
            put_back(t);

            /* this is an identifier. */
            auto assign =
                make_shared<config_ast_simple_expression>();
            assign->simple_value = id;
            assign->ty = HOMESIM_TOKEN_IDENTIFIER;
            expr = assign;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
            /* we're looking ahead, so save the token. */
            put_back(t);

            /* this is an identifier. */
            auto assign =
                make_shared<config_ast_simple_expression>();
            assign->simple_value = id;
            assign->ty = HOMESIM_TOKEN_IDENTIFIER;
            expr = assign;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_PAREN_LEFT, t)
            auto result =
                 parse_inner_expression(id);
            expr = result;
        OPTIONL_END()
    CHOOSE_END()

    return expr;
}

shared_ptr<config_ast_expression>
homesim::parser::parse_inner_expression(const string& id)
{
    shared_ptr<config_ast_expression> expr;

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_NUMBER, t)
            /* this is a complex expression involving a number. */
            auto assign =
                make_shared<config_ast_complex_expression>();
            assign->functor = id;
            assign->args.push_back(t);
            expr = assign;
        OPTIONL_END()
    CHOOSE_END()

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_PAREN_RIGHT)
    SEQUENCE_END()

    return expr;
}

shared_ptr<config_ast_component>
homesim::parser::parse_pull(const string& type)
{
    auto component = make_shared<config_ast_component>();
    component->type = make_shared<string>(type);

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto assign = parse_assign(t.second);
                handle_assignment(component, assign);
            OPTIONL_END(),
        CHOOSE_END()
    }

    return component;
}

shared_ptr<config_ast_scenario> homesim::parser::parse_scenario()
{
    auto scenario = make_shared<config_ast_scenario>();

    SEQUENCE_BEGIN()
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                    scenario->name = t.second;
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_EXECUTION, t)
                auto execution = parse_execution();
                scenario->execution_map.insert(
                    make_pair(execution->name, execution));
            OPTIONL_END()
        CHOOSE_END()
    }

    return scenario;
}

shared_ptr<config_ast_execution> homesim::parser::parse_execution()
{
    auto execution = make_shared<config_ast_execution>();

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            execution->name = t.second;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_NUMBER, t)
            execution->name = t.second;
        OPTIONL_END(),
    CHOOSE_END()

    /* parse the left brace. */
    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_AT, t)
                auto step = parse_step(t.second);
                execution->step_list.push_back(step);
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_AFTER, t)
                auto step = parse_step(t.second);
                execution->step_list.push_back(step);
            OPTIONL_END(),
        CHOOSE_END()
    }

    return execution;
}

shared_ptr<config_ast_step>
homesim::parser::parse_step(const std::string& type)
{
    auto step = make_shared<config_ast_step>();
    step->type = type;

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_START, t)
            auto expression = make_shared<config_ast_simple_expression>();
            expression->ty = HOMESIM_TOKEN_NUMBER;
            expression->simple_value = "0";
            step->step_expression = expression;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            auto expression = parse_complex_expression(t.second);
            step->step_expression = expression;
        OPTIONL_END()
    CHOOSE_END()

    /* parse the left brace. */
    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_BRACE_LEFT)
    SEQUENCE_END()

    bool done = false;

    /* parse each subsequence. */
    while (!done)
    {
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                done = true;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto assign = parse_pin_assign(t.second);
                step->pin_assignments.push_back(assign);
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_ASSERT, t)
                auto assert = parse_assertion(t.second);
                step->assertion_list.push_back(assert);
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_EXPECT, t)
                auto assert = parse_assertion(t.second);
                step->assertion_list.push_back(assert);
            OPTIONL_END(),
        CHOOSE_END()
    }

    return step;
}

shared_ptr<config_ast_assertion>
homesim::parser::parse_assertion(
    const string& type)
{
    auto assertion = make_shared<config_ast_assertion>();
    assertion->type = type;

    SEQUENCE_BEGIN()
        SEQ(HOMESIM_TOKEN_KEYWORD_WIRE),
        SEQ(HOMESIM_TOKEN_DOT),
        SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
            assertion->lhs.push_back(t.second);
        SEQL_END(),
        SEQ(HOMESIM_TOKEN_DOT),
        SEQ(HOMESIM_TOKEN_KEYWORD_SIGNAL),
        SEQ(HOMESIM_TOKEN_EQUALS),
    SEQUENCE_END()

    CHOOSE_BEGIN()
        OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_TRUE, t)
            auto expr = make_shared<config_ast_simple_expression>();
            expr->simple_value = t.second;
            expr->ty = t.first;
            assertion->rhs = expr;
        OPTIONL_END(),
        OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_FALSE, t)
            auto expr = make_shared<config_ast_simple_expression>();
            expr->simple_value = t.second;
            expr->ty = t.first;
            assertion->rhs = expr;
        OPTIONL_END(),
    CHOOSE_END()

    return assertion;
}
