#!/bin/sh

# Main configuration file parser
echo "Main configuration file parser..."
ragel ragel/ConfigFileParser.rl -e -o src/ConfigFileParser.cpp

# Ragel format parser
echo "JSON format parser..."
ragel ragel/JSONParser.rl -e -o include/JSONParser.tcc

# Ragel format parser
echo "LMTP protocol parser..."
ragel ragel/LMTPProtocolParser.rl -e -o src/LMTPProtocolParser.cpp

# Ragel format parser
echo "POP3 protocol parser..."
ragel ragel/POP3ProtocolParser.rl -e -o src/POP3ProtocolParser.cpp

echo "All done."
