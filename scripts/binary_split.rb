#! /usr/bin/env ruby

# ######################################################################## #
# File:     scripts/binary_split.rb
#
# Purpose:  Generates all numbers in a range in a binary-chop order.
#
# Created:  19th March 2025
# Updated:  20th March 2025
#
# Author:   Matthew Wilson
#
# ######################################################################## #


# ######################################################################## #
# requires

require 'libclimate'
require 'highline'


# ######################################################################## #
# includes


# ######################################################################## #
# constants

PROGRAM_VER_MAJOR = 0
PROGRAM_VER_MINOR = 0
PROGRAM_VER_PATCH = 0


# ######################################################################## #
# functions

def generate first, exclusive_last

  num_needed = exclusive_last - first

  return [] if 0 == num_needed
  return [ first ] if 1 == num_needed

  bit_width = num_needed.bit_length
  num_generated = (1 << bit_width) - 1

  r = []

  (0...bit_width).each do |level|

    num_divisions = 2 << level

    delta = (num_generated + 1) / num_divisions

    (1...num_divisions).each do |division|

      next if 0 == (division % 2)

      d = division * delta

      n = (first + d) - 1

      r << n if n < exclusive_last
    end
  end

  r
end


# ######################################################################## #
# command-line handling

options = {}
climate = LibCLImate::Climate.new do |cl|

  cl.info_lines = [

    'collect-c Special and custom Collections and Containers (for C)',
    'Copyright (c) 2025, Matthew Wilson and Synesis Information Systems',
    'Generates numbers in a range as if arrayed in a (mostly) balanced binary tree',
    :version,
    nil,
  ]

  cl.constrain_values = 2
  cl.value_names = %w{ first last }
end

r = climate.run ARGV
first = r.values[0].to_i
last = r.values[1].to_i


# ######################################################################## #
# main

numbers = generate first, last

$stdout.puts "#{numbers}"



# ############################## end of file ############################# #


