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
RecentPrimes=()
LastSave=2

PRIMESFILE="./CurrentPrimes.txt"
STARTFILE="./LastSave.txt"

# Get list of found primes
if test -f "$PRIMESFILE"; then
  RawPrimes=`cat $PRIMESFILE`
  Primes=($RawPrimes)
fi

echo "Loaded ${#Primes[*]} primes"

# Get bookmarked number to start at
if test -f "$STARTFILE"; then
  while IFS= read -r line
  do
    LastSave=("$line")
  done < "$STARTFILE"
fi

Current=$LastSave
echo "Starting at: $Current"


# Begin the hunt!!!
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

  # If we found a prime, add it to the file
  if $isPrime; then
    echo "$Current" >> "$PRIMESFILE"
    echo "$Current" > "$STARTFILE"
    RecentPrimes+=($Current)
  fi

  # Well, maybe bundle it in batches of 5
  if (( ${#RecentPrimes[*]} > 5 )); then
    git commit -am "Found more primes: ${RecentPrimes[*]}"
    git push
    RecentPrimes=()
    LastSave=$Current
  fi

  # Add a save point every 1000 checked numbers
  if (( $Current - $LastSave > 1000 )); then
    echo "$Current" > "$STARTFILE"

    git commit -am "At $Current, nothing in the last 1000"
    git push
    LastSave=$Current
  fi

  ((Current++))
done
