#!/usr/bin/perl

  use v5.36.0;
  use strict;
  use warnings;

  use lib $ENV{'ARPATH'}.'/lib/sys/';
  use Shb7;

  use lib $ENV{'ARPATH'}.'/lib/';
  use Avt;

# ---   *   ---   *   ---

Avt::set_config(

  name  => 'gaoler',
  build => 'ar:gaoler',

  incl  => [qw(bitter)],

);

Avt::scan();
Avt::config();
Avt::make();

# ---   *   ---   *   ---
1; # ret
