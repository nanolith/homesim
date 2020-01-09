/**
 * \file parser/config_ast_complex_expression_eval.cpp
 *
 * \brief Evaluate a complex expression.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#include <homesim/constants.h>
#include <homesim/parser.h>
#include <sstream>

using namespace homesim;
using namespace std;

static double convert_double(string x);
static string convert_string(double x);

static string ns(string arg1);
static string us(string arg1);
static string ms(string arg1);
static string kohms(string arg1);

string homesim::config_ast_complex_expression::eval()
{
    /* TODO - bubble error condition to caller. */

    if (functor == "ns")
    {
        if (args.size() < 1)
            return "0.0";

        return ns(args.front().second);
    }
    else if (functor == "us")
    {
        if (args.size() < 1)
            return "0.0";

        return us(args.front().second);
    }
    else if (functor == "ms")
    {
        if (args.size() < 1)
            return "0.0";

        return ms(args.front().second);
    }
    else if (functor == "kohms")
    {
        if (args.size() < 1)
            return "0.0";

        return kohms(args.front().second);
    }
    else
    {
        return "0.0";
    }
}

static double convert_double(string x)
{
    stringstream in(x);
    double retval;

    in >> retval;

    return retval;
}

static string convert_string(double x)
{
    stringstream out;

    out << x;

    return out.str();
}

static string ns(string arg1)
{
    double x = convert_double(arg1);
    x *= nanoseconds_to_seconds_scale;
    return convert_string(x);
}

static string us(string arg1)
{
    double x = convert_double(arg1);
    x *= microseconds_to_seconds_scale;
    return convert_string(x);
}

static string ms(string arg1)
{
    double x = convert_double(arg1);
    x *= milliseconds_to_seconds_scale;
    return convert_string(x);
}

static string kohms(string arg1)
{
    double x = convert_double(arg1);
    x *= kohms_to_ohms_scale;
    return convert_string(x);
}
