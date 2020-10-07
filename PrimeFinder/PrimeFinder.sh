#!/bin/bash
#
# This program finds primes!
#     But not particularly well, or anything
# 
# Set this to run on start up, it syncs with git
#     picks up where it left off, and periodically commits new findings

echo "Starting PrimeFinder"
git fetch
git pull

Primes=()
LastSave=2

PRIMESFILE="./CurrentPrimes.txt"
STARTFILE="./CurrentStart.txt"

# Get list of found primes
if test -f "$PRIMESFILE"; then
  while IFS= read -r line
  do
    Primes+=("$line")
  done < "$PRIMESFILE"
fi

echo "Current primes are: ${Primes[*]}"

# Get bookmarked number to start at
if test -f "$STARTFILE"; then
  while IFS= read -r line
  do
    LastSave=("$line")
  done < "$STARTFILE"
fi

Current=$LastSave
echo "Starting at: $Current"


# Begin the hunt!
while true
do
  isPrime=true
  for i in "${Primes[@]}"
  do
    if ! (( $Current % $i )); then
      isPrime=false;
      continue
    fi
  done

  if $isPrime; then
    echo "$Current" >> "$PRIMESFILE"
    echo "$Current" > "$STARTFILE"

    # git commit -am "Found another prime... $Current!"
    # git push
    LastSave=$Current
  fi

  if (( $Current - $LastSave > 1000 )); then
    echo "$Current" > "$STARTFILE"

    # git commit -am "At $Current, nothing in the last 1000"
    # git push
    LastSave=$Current
  fi

  ((Current++))
done
