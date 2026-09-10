#! /usr/bin/env ruby

# ######################################################################## #
# File:     scripts/binary_split.rb
#
# Purpose:  Generates all numbers in a range in a binary-chop order.
#
# Created:  19th March 2025
# Updated:  10th September 2026
#
# Author:   Matthew Wilson
#
# ######################################################################## #


# ######################################################################## #
# requires

require 'highline'
require 'libclimate'
require 'xqsr3/extensions/string/map_option_string'


# ######################################################################## #
# includes


# ######################################################################## #
# constants

FORMAT_TYPES = %w{
  C-[c]ompatible-list
  [p]lain-sequence
  [r]uby-list
}

PROGRAM_VER_MAJOR = 0
PROGRAM_VER_MINOR = 0
PROGRAM_VER_PATCH = 1



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

def format_list l, **options

  case options[:format]
  when nil, :plain_sequence

    l.join(' ')
  when :C_compatible_list

    '{ ' + l.join(', ') + ' }'
  when :ruby_list

    l
  else

    (options[:climate] || ::Kernel).abort "VIOLATION: unexpected value for options[:format]"
  end
end


# ######################################################################## #
# command-line handling

options = {}
climate = LibCLImate::Climate.new do |cl|

  cl.add_option('--format', alias: '-f', values: FORMAT_TYPES) do |o, sp|

    options[:format] = o.value.map_option_string(FORMAT_TYPES) or cl.abort "invalid value '#{o.value}' passed to '--format'"
  end

  cl.info_lines = [

    'collect-c Special and custom Collections and Containers (for C)',
    'Copyright (c) 2025-2026, Matthew Wilson and Synesis Information Systems',
    'Generates numbers in a range as if arrayed in a (mostly) balanced binary tree',
    :version,
    nil,
  ]

  cl.constrain_values = 2
  cl.usage_values = '<first> <exclusive-last>'
  cl.value_names = %w{ first last }

  options[:climate] = cl
end

r = climate.run ARGV
first = r.values[0].to_i
last = r.values[1].to_i


# ######################################################################## #
# main

numbers = generate first, last

numbers = format_list numbers, **options

$stdout.puts "#{numbers}"



# ############################## end of file ############################# #


