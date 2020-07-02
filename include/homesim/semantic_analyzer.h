/**
 * \file homesim/semantic_analyzer.h
 *
 * \brief Semantic analyzer for homesim AST.
 *
 * \copyright Copyright 2020 Justin Handville. All rights reserved.
 */
#ifndef  HOMESIM_SEMANTIC_ANALYZER_HEADER_GUARD
# define HOMESIM_SEMANTIC_ANALYZER_HEADER_GUARD

/** C++ version check. */
#if !defined(__cplusplus) || __cplusplus < 201402L
# error This file requires C++14 or greater.
#endif

#include <homesim/parser.h>

namespace homesim {

/* forward declarations. */
class environment;
class simulation;
class schematic;
class component;
class wire;

/**
 * \brief This exception is thrown when an error is found during semantic
 * analysis.
 */
class semantic_error : public std::runtime_error
{
public:
    semantic_error(const std::string& what)
        : runtime_error(what)
    {
    }
};

class semantic_analyzer
{
public:

    /**
     * \brief Semantic analyzer for homesim AST.
     *
     * \param en                The environment for this analysis.
     * \param mod               The module to analyze.
     */
    semantic_analyzer(
        std::shared_ptr<environment> en,
        std::shared_ptr<config_ast_module> mod);

    /**
     * \brief Extract a simulation from the analyzer.
     *
     * \returns a simulation on success.
     *
     * \throws a \ref semantic_analysis_error on failure.
     */
    std::shared_ptr<simulation> extract_simulation();

    /**
     * \brief Extract a schematic from the analyzer.
     *
     * \returns a schematic on success.
     *
     * \throws a \ref semantic_analysis_error on failure.
     */
    std::shared_ptr<schematic> extract_schematic();

    /**
     * \brief Extract a component from the analyzer.
     *
     * \returns a component on success.
     *
     * \throws a \ref semantic_analysis_error on failure.
     */
    std::shared_ptr<component> extract_component();

private:
    std::shared_ptr<environment> env;
    std::shared_ptr<config_ast_module> module;
    std::map<std::string, std::shared_ptr<component>> component_map;
    std::map<std::string, std::shared_ptr<wire>> wire_map;
    bool analyzed;

    /**
     * \brief Perform analysis on the AST, verifying all details.
     *
     * \throws a \ref semantic_error on failure.
     */
    void analyze();
    void analyze_components();
    void analyze_wires();
};

} /* namespace homesim */

#endif /*HOMESIM_SEMANTIC_ANALYZER_HEADER_GUARD*/
