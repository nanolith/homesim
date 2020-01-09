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
parser::parse_result<config_ast_module> homesim::parser::parse()
{
    typedef parse_result<config_ast_module> return_type;

    auto module = make_shared<config_ast_module>();

    /* attempt to parse "module identifier {" */
    auto errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_KEYWORD_MODULE),
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                module->name = t.second;
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = make_shared<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT,t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_COMPONENT,t)
                    auto maybeComponent = parse_component();
                    if (!!maybeComponent.first)
                        errors = maybeComponent.first;
                    else
                        module->component_map.insert(
                            make_pair(
                                maybeComponent.second->name,
                                maybeComponent.second));
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_WIRE,t)
                    auto maybeWire = parse_wire();
                    if (!!maybeWire.first)
                        errors = maybeWire.first;
                    else
                        module->wire_map.insert(
                            make_pair(
                                maybeWire.second->name,
                                maybeWire.second));
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_SIGNAL,t)
                    auto maybeProbe = parse_probe(t.second);
                    if (!!maybeProbe.first)
                        errors = maybeProbe.first;
                    else
                        module->probe_map.insert(
                            make_pair(
                                maybeProbe.second->name,
                                maybeProbe.second));
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_STATE,t)
                    auto maybeProbe = parse_probe(t.second);
                    if (!!maybeProbe.first)
                        errors = maybeProbe.first;
                    else
                        module->probe_map.insert(
                            make_pair(
                                maybeProbe.second->name,
                                maybeProbe.second));
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_SCENARIO,t)
                    auto maybeScenario = parse_scenario();
                    if (!!maybeScenario.first)
                        errors = maybeScenario.first;
                    else
                        module->scenario_map.insert(
                            make_pair(
                                maybeScenario.second->name,
                                maybeScenario.second));
                OPTIONL_END()
            CHOOSE_END()

        if (errors->size() > 0)
            return return_type(errors, nullptr);
        if (!!newErrors)
            return return_type(newErrors, nullptr);
    }

    /* success. */
    return return_type(nullptr, module);
}

parser::parse_result<config_ast_component> homesim::parser::parse_component()
{
    typedef parse_result<config_ast_component> return_type;

    auto component = make_shared<config_ast_component>();

    auto errors =
        SEQUENCE_BEGIN()
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                        component->name = t.second;
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                    auto maybeAssign = parse_assign(t.second);
                    if (!!maybeAssign.first)
                        errors = maybeAssign.first;
                    else
                        handle_assignment(
                            component, maybeAssign.second);
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_TYPE, t)
                    auto maybeType = parse_type();
                    if (!!maybeType.first)
                        errors = maybeType.first;
                    else
                        component->type = maybeType.second;
                OPTIONL_END()
            CHOOSE_END()

        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, component);
}

parser::parse_result<config_ast_wire> homesim::parser::parse_wire()
{
    typedef parse_result<config_ast_wire> return_type;

    auto wire = make_shared<config_ast_wire>();

    auto errors =
        SEQUENCE_BEGIN()
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                wire->name = t.second;
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                    auto maybeConnection = parse_connection(t.second);
                    if (!!maybeConnection.first)
                        errors = maybeConnection.first;
                    else
                        wire->connection_list.push_back(
                            maybeConnection.second);
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_PULLUP, t)
                    auto maybePullup = parse_pull(t.second);
                    if (!!maybePullup.first)
                        errors = maybePullup.first;
                    else
                        wire->pullup_pulldown = maybePullup.second;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_PULLDOWN, t)
                    auto maybePulldown = parse_pull(t.second);
                    if (!!maybePulldown.first)
                        errors = maybePulldown.first;
                    else
                        wire->pullup_pulldown = maybePulldown.second;
                OPTIONL_END()
            CHOOSE_END()
        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, wire);
}

parser::parse_result<config_ast_probe>
homesim::parser::parse_probe(const string& type)
{
    typedef parse_result<config_ast_probe> return_type;

    auto probe = make_shared<config_ast_probe>();
    probe->type = type;

    auto errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_KEYWORD_PROBE),
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                probe->name = t.second;
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_TYPE, t)
                    auto maybeType = parse_type();
                    if (!!maybeType.first)
                        errors = maybeType.first;
                    else
                        probe->sub_type = maybeType.second;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_WIRE, t)
                    auto maybeWireRef = parse_wire_ref();
                    if (!!maybeWireRef.first)
                        errors = maybeWireRef.first;
                    else
                        probe->wire_ref_list.push_back(*maybeWireRef.second);
                OPTIONL_END()
            CHOOSE_END()
        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, probe);
}

parser::parse_result<string> homesim::parser::parse_type()
{
    typedef parse_result<string> return_type;

    string name;

    auto errors =
        SEQUENCE_BEGIN()
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                name = t.second;
            SEQL_END()
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, make_shared<string>(name));
}

parser::parse_result<string> homesim::parser::parse_wire_ref()
{
    typedef parse_result<string> return_type;

    string ref;

    auto errors =
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                ref = t.second;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_ASTERISK, t)
                ref = t.second;
            OPTIONL_END()
        CHOOSE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, make_shared<string>(ref));
}

parser::parse_result<config_ast_assignment>
homesim::parser::parse_assign(
    const string& id)
{
    typedef parse_result<config_ast_assignment> return_type;

    auto assignment = make_shared<config_ast_assignment>();
    assignment->lhs_major = id;

    auto errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_BRACKET_LEFT),
            SEQL_BEGIN(HOMESIM_TOKEN_STRING, t)
                assignment->lhs_minor =
                    trim_string(t.second);
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACKET_RIGHT),
            SEQ(HOMESIM_TOKEN_ASSIGN)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    shared_ptr<error_list> innerErrors;

    errors =
        CHOOSE_BEGIN()
            OPTIONF(HOMESIM_TOKEN_NUMBER, simple_assignment_maker(assignment)),
            OPTIONF(HOMESIM_TOKEN_STRING, simple_assignment_maker(assignment)),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto maybeExpression = parse_complex_expression(t.second);
                if (!!maybeExpression.first)
                {
                    innerErrors = maybeExpression.first;
                }
                else
                {
                    assignment->rhs = maybeExpression.second;
                }
            OPTIONL_END()
        CHOOSE_END()

    if (!!innerErrors)
    {
        errors = innerErrors;
    }

    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, assignment);
}

parser::parse_result<config_ast_assignment>
homesim::parser::parse_pin_assign(
    const string& id)
{
    typedef parse_result<config_ast_assignment> return_type;

    auto assignment = make_shared<config_ast_assignment>();
    assignment->lhs_major = id;

    auto errors =
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
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    shared_ptr<error_list> innerErrors;

    errors =
        CHOOSE_BEGIN()
            OPTIONF(
                HOMESIM_TOKEN_KEYWORD_TRUE,
                simple_assignment_maker(assignment)),
            OPTIONF(
                HOMESIM_TOKEN_KEYWORD_FALSE,
                simple_assignment_maker(assignment)),
        CHOOSE_END()

    if (!!innerErrors)
    {
        errors = innerErrors;
    }

    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, assignment);
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

parser::parse_result<config_ast_connection>
homesim::parser::parse_connection(const string& id)
{
    typedef parse_result<config_ast_connection> return_type;

    auto connection = make_shared<config_ast_connection>();
    connection->component = id;

    auto errors =
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
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, connection);
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

parser::parse_result<config_ast_expression>
homesim::parser::parse_complex_expression(const string& id)
{
    typedef parse_result<config_ast_expression> return_type;
    shared_ptr<config_ast_expression> expr;
    shared_ptr<error_list> innerErrors;

    auto errors =
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
                if (!!result.first)
                {
                    innerErrors = result.first;
                }
                else
                {
                    expr = result.second;
                }
            OPTIONL_END()
        CHOOSE_END()
    if (!!innerErrors)
    {
        errors = innerErrors;
    }

    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, expr);
}

parser::parse_result<config_ast_expression>
homesim::parser::parse_inner_expression(const string& id)
{
    typedef parse_result<config_ast_expression> return_type;
    shared_ptr<config_ast_expression> expr;
    shared_ptr<error_list> innerErrors;

    auto errors =
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
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_PAREN_RIGHT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, expr);
}

parser::parse_result<config_ast_component>
homesim::parser::parse_pull(const string& type)
{
    typedef parse_result<config_ast_component> return_type;

    auto component = make_shared<config_ast_component>();
    component->type = make_shared<string>(type);

    auto errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                    auto maybeAssign = parse_assign(t.second);
                    if (!!maybeAssign.first)
                        errors = maybeAssign.first;
                    else
                        handle_assignment(
                            component, maybeAssign.second);
                OPTIONL_END(),
            CHOOSE_END()

        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, component);
}

parser::parse_result<config_ast_scenario> homesim::parser::parse_scenario()
{
    typedef parse_result<config_ast_scenario> return_type;

    auto scenario = make_shared<config_ast_scenario>();

    auto errors =
        SEQUENCE_BEGIN()
            SEQL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                        scenario->name = t.second;
            SEQL_END(),
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_EXECUTION, t)
                    auto maybeExecution = parse_execution();
                    if (!!maybeExecution.first)
                        errors = maybeExecution.first;
                    else
                        scenario->execution_map.insert(
                            make_pair(
                                maybeExecution.second->name,
                                maybeExecution.second));
                OPTIONL_END()
            CHOOSE_END()

        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, scenario);
}

parser::parse_result<config_ast_execution> homesim::parser::parse_execution()
{
    typedef parse_result<config_ast_execution> return_type;

    auto execution = make_shared<config_ast_execution>();

    auto errors =
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                execution->name = t.second;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_NUMBER, t)
                execution->name = t.second;
            OPTIONL_END(),
        CHOOSE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    /* parse the left brace. */
    errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_AT, t)
                    auto maybeStep = parse_step(t.second);
                    if (!!maybeStep.first)
                        errors = maybeStep.first;
                    else
                        execution->step_list.push_back(maybeStep.second);
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_AFTER, t)
                    auto maybeStep = parse_step(t.second);
                    if (!!maybeStep.first)
                        errors = maybeStep.first;
                    else
                        execution->step_list.push_back(maybeStep.second);
                OPTIONL_END(),
            CHOOSE_END()

        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, execution);
}

parser::parse_result<config_ast_step>
homesim::parser::parse_step(const std::string& type)
{
    typedef parse_result<config_ast_step> return_type;

    auto step = make_shared<config_ast_step>();
    step->type = type;

    shared_ptr<error_list> innerErrors;

    auto errors =
        CHOOSE_BEGIN()
            OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_START, t)
                auto expression = make_shared<config_ast_simple_expression>();
                expression->ty = HOMESIM_TOKEN_NUMBER;
                expression->simple_value = "0";
                step->step_expression = expression;
            OPTIONL_END(),
            OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                auto maybeExpression = parse_complex_expression(t.second);
                if (!!maybeExpression.first)
                {
                    innerErrors = maybeExpression.first;
                }
                else
                {
                    step->step_expression = maybeExpression.second;
                }
            OPTIONL_END()
        CHOOSE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    /* parse the left brace. */
    errors =
        SEQUENCE_BEGIN()
            SEQ(HOMESIM_TOKEN_BRACE_LEFT)
        SEQUENCE_END()
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    bool done = false;
    errors = shared_ptr<error_list>();

    /* parse each subsequence. */
    while (!done)
    {
        auto newErrors =
            CHOOSE_BEGIN()
                OPTIONL_BEGIN(HOMESIM_TOKEN_BRACE_RIGHT, t)
                    done = true;
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_IDENTIFIER, t)
                    auto maybeAssign = parse_pin_assign(t.second);
                    if (!!maybeAssign.first)
                        errors = maybeAssign.first;
                    else
                        step->pin_assignments.push_back(maybeAssign.second);
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_ASSERT, t)
                    auto maybeAssert = parse_assertion(t.second);
                    if (!!maybeAssert.first)
                        errors = maybeAssert.first;
                    else
                        step->assertion_list.push_back(maybeAssert.second);
                OPTIONL_END(),
                OPTIONL_BEGIN(HOMESIM_TOKEN_KEYWORD_EXPECT, t)
                    auto maybeAssert = parse_assertion(t.second);
                    if (!!maybeAssert.first)
                        errors = maybeAssert.first;
                    else
                        step->assertion_list.push_back(maybeAssert.second);
                OPTIONL_END(),
            CHOOSE_END()

        if (!!newErrors && !errors)
        {
            errors = newErrors;
        }

        if (!!errors)
        {
            return return_type(errors, nullptr);
        }
    }

    return return_type(nullptr, step);
}

parser::parse_result<config_ast_assertion>
homesim::parser::parse_assertion(
    const string& type)
{
    typedef parse_result<config_ast_assertion> return_type;

    auto assertion = make_shared<config_ast_assertion>();
    assertion->type = type;

    auto errors =
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
    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    shared_ptr<error_list> innerErrors;

    errors =
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

    if (!!innerErrors)
    {
        errors = innerErrors;
    }

    if (!!errors)
    {
        return return_type(errors, nullptr);
    }

    return return_type(nullptr, assertion);
}

