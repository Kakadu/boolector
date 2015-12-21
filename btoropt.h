/*  Boolector: Satisfiablity Modulo Theories (SMT) solver.
 *
 *  Copyright (C) 2014-2015 Aina Niemetz.
 *  Copyright (C) 2014-2015 Mathias Preiner.
 *  Copyright (C) 2014-2015 Armin Biere.
 *
 *  All rights reserved.
 *
 *  This file is part of Boolector.
 *  See COPYING for more information on using this software.
 */

#ifndef BTOROPTS_H_INCLUDED
#define BTOROPTS_H_INCLUDED

#include <stdint.h>

#define BTOR_VERBOSITY_MAX 4

#define BTOR_ENGINE_CORE 0
#define BTOR_ENGINE_SLS 1
#define BTOR_ENGINE_EF 2
#define BTOR_ENGINE_DFLT BTOR_ENGINE_CORE
#define BTOR_ENGINE_MIN BTOR_ENGINE_CORE
#define BTOR_ENGINE_MAX BTOR_ENGINE_EF

#define BTOR_INPUT_FORMAT_NONE 0
#define BTOR_INPUT_FORMAT_BTOR 1
#define BTOR_INPUT_FORMAT_BTOR2 2
#define BTOR_INPUT_FORMAT_SMT1 3
#define BTOR_INPUT_FORMAT_SMT2 4
#define BTOR_INPUT_FORMAT_DFLT BTOR_INPUT_FORMAT_NONE
#define BTOR_INPUT_FORMAT_MIN BTOR_INPUT_FORMAT_NONE
#define BTOR_INPUT_FORMAT_MAX BTOR_INPUT_FORMAT_SMT2

#define BTOR_OUTPUT_BASE_BIN 0
#define BTOR_OUTPUT_BASE_HEX 1
#define BTOR_OUTPUT_BASE_DEC 2
#define BTOR_OUTPUT_BASE_DFLT BTOR_OUTPUT_BASE_BIN
#define BTOR_OUTPUT_BASE_MIN BTOR_OUTPUT_BASE_BIN
#define BTOR_OUTPUT_BASE_MAX BTOR_OUTPUT_BASE_DEC

#define BTOR_OUTPUT_FORMAT_BTOR 1
#define BTOR_OUTPUT_FORMAT_BTOR2 2
#define BTOR_OUTPUT_FORMAT_SMT2 3
#define BTOR_OUTPUT_FORMAT_AIGER_ASCII 4
#define BTOR_OUTPUT_FORMAT_AIGER_BINARY 5
#define BTOR_OUTPUT_FORMAT_DFLT BTOR_OUTPUT_FORMAT_BTOR
#define BTOR_OUTPUT_FORMAT_MIN BTOR_OUTPUT_FORMAT_BTOR
#define BTOR_OUTPUT_FORMAT_MAX BTOR_OUTPUT_FORMAT_AIGER_BINARY

#define BTOR_JUST_HEUR_LEFT 0
#define BTOR_JUST_HEUR_BRANCH_MIN_APP 1
#define BTOR_JUST_HEUR_BRANCH_MIN_DEP 2
#define BTOR_JUST_HEUR_DFLT BTOR_JUST_HEUR_BRANCH_MIN_APP
#define BTOR_JUST_HEUR_MIN BTOR_JUST_HEUR_LEFT
#define BTOR_JUST_HEUR_MAX BTOR_JUST_HEUR_BRANCH_MIN_DEP

#define BTOR_SLS_STRAT_BEST_MOVE 0
#define BTOR_SLS_STRAT_PROB_RAND_WALK 1
#define BTOR_SLS_STRAT_FIRST_BEST_MOVE 2
#define BTOR_SLS_STRAT_BEST_SAME_MOVE 3
#define BTOR_SLS_STRAT_ALWAYS_PROP 4
#define BTOR_SLS_STRAT_DFLT BTOR_SLS_STRAT_BEST_MOVE
#define BTOR_SLS_STRAT_MIN 0
#define BTOR_SLS_STRAT_MAX 4

typedef struct BtorOpt
{
  int internal;     /* internal option? */
  const char *shrt; /* short option identifier (may be 0) */
  const char *lng;  /* long option identifier */
  const char *desc; /* description */
  uint32_t val;     /* value */
  uint32_t dflt;    /* default value */
  uint32_t min;     /* min value */
  uint32_t max;     /* max value */
} BtorOpt;

#define BTOR_OPT_ENGINE "engine"
#define BTOR_OPT_MODEL_GEN "model_gen"
#define BTOR_OPT_INCREMENTAL "incremental"
#define BTOR_OPT_INCREMENTAL_ALL "incremental_all"
#define BTOR_OPT_INCREMENTAL_IN_DEPTH "incremental_in_depth"
#define BTOR_OPT_INCREMENTAL_LOOK_AHEAD "incremental_look_ahead"
#define BTOR_OPT_INCREMENTAL_INTERVAL "incremental_interval"
#define BTOR_OPT_INPUT_FORMAT "input_format"
#define BTOR_OPT_OUTPUT_NUMBER_FORMAT "output_number_format"
#define BTOR_OPT_OUTPUT_FORMAT "output_format"
#define BTOR_OPT_REWRITE_LEVEL "rewrite_level"
#define BTOR_OPT_BETA_REDUCE_ALL "beta_reduce_all"
#define BTOR_OPT_ACKERMANN "ackermannize"
#define BTOR_OPT_DUAL_PROP "dual_prop"
#define BTOR_OPT_JUST "just"
#define BTOR_OPT_SLS "sls"
#define BTOR_OPT_SLS_STRATEGY "sls_strategy"
#define BTOR_OPT_SLS_MOVE_GW "sls_move_gw"
#define BTOR_OPT_SLS_MOVE_RANGE "sls_move_range"
#define BTOR_OPT_SLS_MOVE_SEGMENT "sls_move_segment"
#define BTOR_OPT_SLS_MOVE_RAND_WALK "sls_move_rand_walk"
#define BTOR_OPT_SLS_MOVE_RAND_WALK_PROB "sls_move_rand_walk_prob"
#define BTOR_OPT_SLS_MOVE_RANDOMIZEALL "sls_move_rand_all"
#define BTOR_OPT_SLS_MOVE_RANDOMIZERANGE "sls_move_rand_range"
#define BTOR_OPT_SLS_MOVE_PROP "sls_move_prop"
#define BTOR_OPT_SLS_MOVE_PROP_N_PROP "sls_move_prop_n_prop"
#define BTOR_OPT_SLS_MOVE_PROP_N_SLS "sls_move_prop_n_sls"
#define BTOR_OPT_SLS_MOVE_PROP_FORCE_RW "sls_move_prop_force_rw"
#define BTOR_OPT_SLS_MOVE_PROP_NO_FLIP_COND "sls_move_prop_no_flip_cond"
#define BTOR_OPT_SLS_MOVE_PROP_FLIP_COND_PROB "sls_move_prop_flip_cond_prob"
#define BTOR_OPT_SLS_MOVE_INC_MOVE_TEST "sls_move_inc_move_test"
#define BTOR_OPT_SLS_USE_RESTARTS "sls_use_restarts"
#define BTOR_OPT_SLS_USE_BANDIT "sls_use_bandit"
#ifndef BTOR_DO_NOT_OPTIMIZE_UNCONSTRAINED
#define BTOR_OPT_UCOPT "ucopt"
#endif
#define BTOR_OPT_AUTO_CLEANUP "auto_cleanup"
#define BTOR_OPT_PRETTY_PRINT "pretty_print"
#define BTOR_OPT_EXIT_CODES "exit_codes"
#define BTOR_OPT_LOGLEVEL "loglevel"
#define BTOR_OPT_VERBOSITY "verbosity"
#define BTOR_OPT_SEED "seed"
#define BTOR_OPT_SIMPLIFY_CONSTRAINTS "simplify_constraints"
#define BTOR_OPT_AUTO_CLEANUP_INTERNAL "auto_cleanup_internal"
#ifdef BTOR_CHECK_FAILED
#define BTOR_OPT_CHK_FAILED_ASSUMPTIONS "chk_failed_assumptions"
#endif
#define BTOR_OPT_UCOPT "ucopt"
#define BTOR_OPT_LAZY_SYNTHESIZE "lazy_synthesize"
#define BTOR_OPT_ELIMINATE_SLICES "eliminate_slices"
#define BTOR_OPT_DELAY_LEMMAS "delay_lemmas"
#define BTOR_OPT_JUST_HEURISTIC "just_heuristic"
#define BTOR_OPT_PARSE_INTERACTIVE "parse_interactive"
#define BTOR_OPT_MERGE_LAMBDAS "merge_lambdas"
#define BTOR_OPT_EXTRACT_LAMBDAS "extract_lambdas"
#define BTOR_OPT_SKELETON_PREPROC "skeleton_preproc"
#define BTOR_OPT_SORT_EXP "sort_exp"
#define BTOR_OPT_SORT_AIG "sort_aig"
#define BTOR_OPT_SORT_AIGVEC "sort_aigvec"
#define BTOR_OPT_RW_NORMALIZE "rw_normalize"
#define BTOR_OPT_VAR_SUBST "var_subst"

typedef struct BtorOpts
{
  BtorOpt first; /* dummy for iteration */
  /* ----------------------------------------------------------------------- */
  BtorOpt engine; /* Boolector engine */

  BtorOpt model_gen; /* model generation enabled */

  BtorOpt incremental;     /* incremental usage */
  BtorOpt incremental_all; /* incremental usage, solve all */

  BtorOpt input_format; /* force input format */

  BtorOpt output_number_format; /* output number format */
  BtorOpt output_format;        /* output file format */

  BtorOpt rewrite_level;

  BtorOpt beta_reduce_all; /* eagerly eliminate lambda expressions */
  BtorOpt ackermannize;    /* add ackermann constraints */

  BtorOpt sls_strategy;
  BtorOpt sls_move_gw;
  BtorOpt sls_move_range;          /* enable range flip neighbors */
  BtorOpt sls_move_segment;        /* enable segment flip neighbors */
  BtorOpt sls_move_rand_all;       /* randomize all candidates
                                      (rather than just one) */
  BtorOpt sls_move_rand_range;     /* ranomize range-wise and choose best guess
                                      (rather than randomizing all bits) */
  BtorOpt sls_move_rand_walk;      /* enable random walks */
  BtorOpt sls_move_rand_walk_prob; /* probability to choose a random walk */
  BtorOpt sls_move_prop;           /* enable propagation moves */
  BtorOpt sls_move_prop_n_prop;    /* number of prop moves (vs. sls moves) */
  BtorOpt sls_move_prop_n_sls;     /* number of prop moves (vs. sls moves) */
  BtorOpt sls_move_prop_force_rw;  /* force random walk if prop move fails */
  BtorOpt sls_move_prop_no_flip_cond;   /* do not choose cond flip during
                                     path selection for prop moves */
  BtorOpt sls_move_prop_flip_cond_prob; /* probability to choose cond flip
                                     during path selection for prop moves */
  BtorOpt sls_move_inc_move_test;

  BtorOpt sls_use_restarts;
  BtorOpt sls_use_bandit;

  BtorOpt dual_prop;      /* dual prop optimization */
  BtorOpt just;           /* justification optimization */
  BtorOpt just_heuristic; /* use heuristic (else: input [0] if both
                             are controlling) */
#ifndef BTOR_DO_NOT_OPTIMIZE_UNCONSTRAINED
  BtorOpt ucopt; /* unconstrained optimization */
#endif
  BtorOpt lazy_synthesize;  /* lazily synthesize expressions */
  BtorOpt eliminate_slices; /* eliminate slices on variables */
  BtorOpt eager_lemmas;     /* eager lemma generation */
  BtorOpt merge_lambdas;    /* merge lambda chains */
  BtorOpt extract_lambdas;  /* extract lambda terms */
  BtorOpt skeleton_preproc; /* skeleton preprocessing */
  BtorOpt var_subst;        /* variable substitution */
  BtorOpt sort_exp;         /* sort commutative expression nodes */
  BtorOpt sort_aig;         /* sort AIG nodes */
  BtorOpt sort_aigvec;      /* sort AIG vectors */

  BtorOpt auto_cleanup; /* automatic cleanup of exps, assignment
                           strings (external references only) */
  BtorOpt pretty_print; /* reindex exps and sorts when dumping */
  BtorOpt exit_codes;   /* use Boolector exit codes rather than
                           returning 0 on success and 1 on error */
#ifndef NBTORLOG
  BtorOpt loglevel;
#endif
  BtorOpt verbosity;

  BtorOpt seed; /* seed for random number generator */
  /* internal */
  BtorOpt simplify_constraints;  /* force constraints to true/false */
  BtorOpt auto_cleanup_internal; /* force cleanup of exps, assignm. strings
                                    (internal references only) */

  BtorOpt incremental_in_depth;   /* incremental usage, in-depth mode */
  BtorOpt incremental_look_ahead; /* incremental usage, look-ahead mode */
  BtorOpt incremental_interval;   /* incremental usage, interval mode */
  BtorOpt parse_interactive;      /* interactive parse mode */
  BtorOpt rw_normalize;           /* normalization during rewriting */

  /* ----------------------------------------------------------------------- */
  BtorOpt last; /* dummy for iteration */
#ifdef BTOR_CHECK_FAILED
  BtorOpt chk_failed_assumptions;
#endif

} BtorOpts;

struct Btor;

void btor_init_opts (struct Btor *btor);

void btor_set_opt (struct Btor *btor, const char *name, uint32_t val);

/* does not assert existing opt with name 'name',
 * not for boolector internal use */
BtorOpt *btor_get_opt_aux (struct Btor *btor,
                           const char *name,
                           int skip_internal);
/* asserts existing opt with name 'opt' */
BtorOpt *btor_get_opt (struct Btor *btor, const char *name);

int btor_get_opt_val (struct Btor *btor, const char *name);
int btor_get_opt_min (struct Btor *btor, const char *name);
int btor_get_opt_max (struct Btor *btor, const char *name);
int btor_get_opt_dflt (struct Btor *btor, const char *name);
const char *btor_get_opt_shrt (struct Btor *btor, const char *name);
const char *btor_get_opt_desc (struct Btor *btor, const char *name);

const char *btor_first_opt (struct Btor *btor);
const char *btor_next_opt (struct Btor *btor, const char *cur);
#endif
