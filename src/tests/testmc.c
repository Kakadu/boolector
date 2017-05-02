/*  Boolector: Satisfiablity Modulo Theories (SMT) solver.
 *
 *  Copyright (C) 2013-2016 Armin Biere.
 *  Copyright (C) 2016 Aina Niemetz.
 *
 *  All rights reserved.
 *
 *  This file is part of Boolector.
 *  See COPYING for more information on using this software.
 */

#include "testmc.h"
#include "btormc.h"
#include "btornode.h"
#include "testrunner.h"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

static BtorMC *g_mc;
static Btor *g_btor;

void
init_mc_tests (void)
{
  assert (!g_mc);
  assert (!g_btor);
}

void
finish_mc_tests (void)
{
  assert (!g_mc);
  assert (!g_btor);
}

/*------------------------------------------------------------------------*/

static void
init_mc_test (void)
{
  assert (!g_mc);
  g_mc = boolector_mc_new ();
  assert (!g_btor);
  g_btor = boolector_mc_btor (g_mc);
  assert (g_btor);
}

static void
finish_mc_test (void)
{
  g_btor = 0;
  boolector_mc_delete (g_mc);
  g_mc = 0;
}

/*------------------------------------------------------------------------*/

static void
test_mcnewdel ()
{
  init_mc_test ();
  finish_mc_test ();
}

/*------------------------------------------------------------------------*/

#define PRINT(NAME, TIME)                                   \
  do                                                        \
  {                                                         \
    char *VAL = boolector_mc_assignment (g_mc, NAME, TIME); \
    assert (VAL);                                           \
    fprintf (file, #NAME " = %s\n", VAL);                   \
    fflush (file);                                          \
    boolector_mc_free_assignment (g_mc, VAL);               \
  } while (0)

static void
test_mctoggle ()
{
  int i, k, mode;
  BoolectorNode *bit, *one, *zero, *add, *bad;
  char *fname, *suffix = "mctoggle.log";
  FILE *file;
  BoolectorSort s;

  for (mode = 0; mode < 2; mode++)
  {
    init_mc_test ();
    s = boolector_bitvec_sort (g_btor, 1);
    if (mode) boolector_mc_enable_trace_gen (g_mc);

    // boolector_mc_set_verbosity (g_mc, 3);

    bit  = boolector_latch (g_mc, 1, "counter");
    one  = boolector_one (g_btor, s);
    zero = boolector_zero (g_btor, s);
    add  = boolector_add (g_btor, bit, one);
    bad  = boolector_eq (g_btor, bit, one);

    boolector_next (g_mc, bit, add);
    boolector_mc_init (g_mc, bit, zero);
    boolector_bad (g_mc, bad);

    boolector_release (g_btor, one);
    boolector_release (g_btor, zero);
    boolector_release (g_btor, add);
    boolector_release (g_btor, bad);

    k = boolector_mc_bmc (g_mc, 0, 0);
    assert (k < 0);  // can not reach bad within k=0 steps

    k = boolector_mc_bmc (g_mc, 0, 1);
    assert (0 <= k && k <= 1);  // bad reached within k=1 steps

    if (mode)
    {
      fname = (char *) malloc (sizeof (char)
                               * (strlen (BTOR_LOG_DIR) + strlen (suffix) + 1));
      sprintf (fname, "%s%s", BTOR_LOG_DIR, suffix);
      file = fopen (fname, "w");
      assert (file);
      fprintf (file, "Bad state property satisfied at k = %d:\n", k);
      for (i = 0; i <= k; i++)
      {
        fprintf (file, "\n");
        fprintf (file, "[ state at time %d ]\n", i);
        PRINT (bit, i);
      }
      fclose (file);
      free (fname);
    }

    boolector_release_sort (g_btor, s);
    finish_mc_test ();
  }
}

static void
test_mccount2enable ()
{
  int i, k, mode;
  BoolectorNode *counter;  // 2-bit state
  BoolectorNode *enable;   // one boolean control input
  BoolectorNode *one, *zero, *three, *add, *ifenable, *bad;
  char *fname, *suffix = "mccount2enable.log";
  FILE *file;
  BoolectorSort s;

  for (mode = 0; mode < 2; mode++)
  {
    init_mc_test ();
    s = boolector_bitvec_sort (g_btor, 2);

    if (mode) boolector_mc_enable_trace_gen (g_mc);

    // boolector_mc_set_verbosity (g_mc, 3);

    counter = boolector_latch (g_mc, 2, "counter");
    enable  = boolector_input (g_mc, 1, "enable");

    one      = boolector_one (g_btor, s);
    zero     = boolector_zero (g_btor, s);
    three    = boolector_const (g_btor, "11");
    add      = boolector_add (g_btor, counter, one);
    ifenable = boolector_cond (g_btor, enable, add, counter);
    bad      = boolector_eq (g_btor, counter, three);

    boolector_next (g_mc, counter, ifenable);
    boolector_mc_init (g_mc, counter, zero);
    boolector_bad (g_mc, bad);

    boolector_release (g_btor, one);
    boolector_release (g_btor, zero);
    boolector_release (g_btor, three);
    boolector_release (g_btor, add);
    boolector_release (g_btor, ifenable);
    boolector_release (g_btor, bad);

    k = boolector_mc_bmc (g_mc, 0, 1);
    assert (k < 0);  // can not reach bad within k=1 steps

    k = boolector_mc_bmc (g_mc, 0, 5);
    assert (0 <= k && k <= 5);  // bad reached within k=4 steps

    if (mode)
    {
      fname = (char *) malloc (sizeof (char)
                               * (strlen (BTOR_LOG_DIR) + strlen (suffix) + 1));
      sprintf (fname, "%s%s", BTOR_LOG_DIR, suffix);
      file = fopen (fname, "w");
      assert (file);
      boolector_mc_dump (g_mc, file);
      fflush (file);
      fprintf (file, "Bad state property satisfied at k = %d:\n", k);
      for (i = 0; i <= k; i++)
      {
        fprintf (file, "\n");
        fprintf (file, "[ state at time %d ]\n", i);
        PRINT (counter, i);
        fprintf (file, "\n");
        fprintf (file, "[ input at time %d ]\n", i);
        PRINT (enable, i);
      }
      fclose (file);
      free (fname);
    }

    boolector_release_sort (g_btor, s);
    finish_mc_test ();
  }
}

static void
test_mccount2resetenable ()
{
  int k, i;
  BoolectorNode *one, *zero, *three, *add, *ifenable, *ifreset, *bad;
  BoolectorNode *counter;         // 2-bit state
  BoolectorNode *enable, *reset;  // two boolean control inputs
  char *fname, *suffix = "mccount2resetenable.log";
  FILE *file;
  BoolectorSort s;

  init_mc_test ();

  boolector_mc_enable_trace_gen (g_mc);
  // boolector_mc_set_verbosity (g_mc, 3);

  counter = boolector_latch (g_mc, 2, "counter");
  enable  = boolector_input (g_mc, 1, "enable");
  reset   = boolector_input (g_mc, 1, "reset");

  s    = boolector_bitvec_sort (g_btor, 2);
  one  = boolector_one (g_btor, s);
  zero = boolector_zero (g_btor, s);
  boolector_release_sort (g_btor, s);
  three    = boolector_const (g_btor, "11");
  add      = boolector_add (g_btor, counter, one);
  ifenable = boolector_cond (g_btor, enable, add, counter);
  ifreset  = boolector_cond (g_btor, reset, ifenable, zero);
  bad      = boolector_eq (g_btor, counter, three);

  boolector_next (g_mc, counter, ifreset);
  boolector_mc_init (g_mc, counter, zero);
  boolector_bad (g_mc, bad);
  boolector_release (g_btor, one);
  boolector_release (g_btor, zero);
  boolector_release (g_btor, three);
  boolector_release (g_btor, add);
  boolector_release (g_btor, ifenable);
  boolector_release (g_btor, ifreset);
  boolector_release (g_btor, bad);

  k = boolector_mc_bmc (g_mc, 0, 2);
  assert (k < 0);  // can not reach bad within k=1 steps

  k = boolector_mc_bmc (g_mc, 0, 4);
  assert (0 <= k && k <= 4);  // bad reached within k=4 steps

  fname = (char *) malloc (sizeof (char)
                           * (strlen (BTOR_LOG_DIR) + strlen (suffix) + 1));
  sprintf (fname, "%s%s", BTOR_LOG_DIR, suffix);
  file = fopen (fname, "w");
  assert (file);
  fprintf (file, "Bad state property satisfied at k = %d:\n", k);
  for (i = 0; i <= k; i++)
  {
    fprintf (file, "\n");
    fprintf (file, "[ state at time %d ]\n", i);
    PRINT (counter, i);
    fprintf (file, "[ input at time %d ]\n", i);
    PRINT (reset, i);
    PRINT (enable, i);
  }
  fclose (file);
  free (fname);

  finish_mc_test ();
}

#if 0
static void
test_mctwostepsmodel () 
{
  int k, i;
  FILE * file;
  BoolectorNode * zero, * one;
  BoolectorNode * a, * b, * t, * n, * or, * xor;
  BoolectorNode * nexta, * nexta1, * nexta2;
  BoolectorNode * nextb, * nextb1, * nextb2;
  BoolectorNode * bad, * bada, *badb;
  char *fname, *suffix = "mctwostepsmodel.log";

  init_mc_test ();

  boolector_mc_enable_trace_gen (g_mc);
  boolector_mc_set_verbosity (g_mc, 3);

  a = boolector_latch (g_mc, 1, "a");
  b = boolector_latch (g_mc, 1, "b");

  or = boolector_or (g_btor, a, b);	// dangling ...
  xor = boolector_xor (g_btor, a, b);	// dangling ...

  one = boolector_ones (g_btor, 1);
  zero = boolector_zero (g_btor, 1);

  boolector_mc_init (g_mc, a, zero);
  boolector_mc_init (g_mc, b, zero);

  t = boolector_input (g_mc, 1, "t");
  n = boolector_not (g_btor, t);

  nexta1 = boolector_nor (g_btor, t, a);
  nexta2 = boolector_implies (g_btor, n, a);
  nexta = boolector_and (g_btor, nexta1, nexta2);

  nextb1 = boolector_nor (g_btor, n, b);
  nextb2 = boolector_implies (g_btor, t, b);
  nextb = boolector_and (g_btor, nextb1, nextb2);

  boolector_next (g_mc, a, nexta);
  boolector_next (g_mc, b, nextb);

  bada = boolector_eq (g_btor, a, one);
  badb = boolector_eq (g_btor, b, one);
  bad = boolector_and (g_btor, bada, badb);

  boolector_bad (g_mc, bad);

  k = boolector_mc_bmc (g_mc, 0, 2);
  assert (k == 2);			// can reach bad within k=2 steps

  fname = (char *) malloc (
      sizeof (char) * (strlen (BTOR_LOG_DIR) + strlen (suffix) + 1));
  sprintf (fname, "%s%s", BTOR_LOG_DIR, suffix);
  file = fopen (fname, "w");
  assert (file);
  fprintf (file, "Bad state property satisfied at k = %d:\n", k);
  for (i = 0; i <= k; i++)
    {
      fprintf (file, "\n");
      fprintf (file, "[ state at time %d ]\n", i);
      fprintf (file, "\n");
      PRINT (a, i);
      PRINT (b, i);
      fprintf (file, "\n");
      fprintf (file, "[ input at time %d ]\n", i);
      fprintf (file, "\n");
      PRINT (t, i);
      PRINT (n, i);
      fprintf (file, "\n");
      fprintf (file, "[ logic at time %d ]\n", i);
      fprintf (file, "\n");
      PRINT (nexta1, i);
      PRINT (nexta2, i);
      PRINT (nexta, i);
      PRINT (nextb1, i);
      PRINT (nextb2, i);
      PRINT (nextb, i);
      fprintf (file, "\n");
      fprintf (file, "[ dangling logic at time %d ]\n", i);
      fprintf (file, "\n");
      PRINT (or, i);
      PRINT (xor, i);
      fprintf (file, "\n");
      fprintf (file, "[ output at time %d ]\n", i);
      fprintf (file, "\n");
      PRINT (bada, i);
      PRINT (badb, i);
      PRINT (bad, i);
    }
  fclose (file);
  free (fname);

  boolector_release (g_btor, or);
  boolector_release (g_btor, xor);
  boolector_release (g_btor, one);
  boolector_release (g_btor, zero);
  boolector_release (g_btor, n);
  boolector_release (g_btor, nexta1);
  boolector_release (g_btor, nexta2);
  boolector_release (g_btor, nexta);
  boolector_release (g_btor, nextb1);
  boolector_release (g_btor, nextb2);
  boolector_release (g_btor, nextb);
  boolector_release (g_btor, bad);
  boolector_release (g_btor, bada);
  boolector_release (g_btor, badb);

  finish_mc_test ();
}
#endif

static int test_mccount2multi_reached[4];

static void
test_mccount2multi_call_back (void *state, int i, int k)
{
  assert (test_mccount2multi_reached == (int *) state);
  assert (0 <= i), assert (i < 4);
  assert (k >= 0);
  assert (test_mccount2multi_reached[i] == -1);
  test_mccount2multi_reached[i] = k;
#if 0
  printf ("property %d reached at bound %d\n", i, k);
  fflush (stdout);
#endif
}

static void
test_mccount2multi ()
{
  int i, k;
  BoolectorSort s;

  init_mc_test ();
  // boolector_mc_set_verbosity (g_mc, 3);
  boolector_mc_set_stop_at_first_reached_property (g_mc, 0);

  BoolectorNode *count, *one, *zero, *two, *three, *next;
  BoolectorNode *eqzero, *eqone, *eqtwo, *eqthree;
  count = boolector_latch (g_mc, 2, "count");
  s     = boolector_bitvec_sort (g_btor, 2);
  one   = boolector_one (g_btor, s);
  zero  = boolector_zero (g_btor, s);
  boolector_release_sort (g_btor, s);
  two   = boolector_const (g_btor, "10");
  three = boolector_const (g_btor, "11");
  next  = boolector_add (g_btor, count, one);
  boolector_mc_init (g_mc, count, zero);
  boolector_next (g_mc, count, next);
  eqzero  = boolector_eq (g_btor, count, zero);
  eqone   = boolector_eq (g_btor, count, one);
  eqtwo   = boolector_eq (g_btor, count, two);
  eqthree = boolector_eq (g_btor, count, three);
  i       = boolector_bad (g_mc, eqzero);
  assert (i == 0);
  i = boolector_bad (g_mc, eqone);
  assert (i == 1);
  i = boolector_bad (g_mc, eqtwo);
  assert (i == 2);
  i = boolector_bad (g_mc, eqthree);
  assert (i == 3);
  boolector_release (g_btor, one);
  boolector_release (g_btor, zero);
  boolector_release (g_btor, two);
  boolector_release (g_btor, three);
  boolector_release (g_btor, eqone);
  boolector_release (g_btor, eqzero);
  boolector_release (g_btor, eqtwo);
  boolector_release (g_btor, eqthree);
  boolector_release (g_btor, next);

  test_mccount2multi_reached[0] = -1;
  test_mccount2multi_reached[1] = -1;
  test_mccount2multi_reached[2] = -1;
  test_mccount2multi_reached[3] = -1;
  boolector_mc_set_reached_at_bound_call_back (
      g_mc, test_mccount2multi_reached, test_mccount2multi_call_back);
  k = boolector_mc_bmc (g_mc, 2, 3);
  assert (k == 3);
  assert (test_mccount2multi_reached[0] == -1);
  assert (test_mccount2multi_reached[1] == -1);
  assert (test_mccount2multi_reached[2] == 2);
  assert (test_mccount2multi_reached[3] == 3);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 0) < 0);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 1) < 0);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 2) == 2);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 3) == 3);
  k = boolector_mc_bmc (g_mc, 4, 10);
  assert (k == 5);
  assert (test_mccount2multi_reached[0] == 4);
  assert (test_mccount2multi_reached[1] == 5);
  assert (test_mccount2multi_reached[2] == 2);
  assert (test_mccount2multi_reached[3] == 3);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 0) == 4);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 1) == 5);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 2) == 2);
  assert (boolector_mc_reached_bad_at_bound (g_mc, 3) == 3);
  finish_mc_test ();
}

void
run_mc_tests (int argc, char **argv)
{
  BTOR_RUN_TEST (mcnewdel);
  BTOR_RUN_TEST (mctoggle);
  BTOR_RUN_TEST (mccount2enable);
  BTOR_RUN_TEST (mccount2resetenable);
  // BTOR_RUN_TEST (mctwostepsmodel);
  BTOR_RUN_TEST (mccount2multi);
}
