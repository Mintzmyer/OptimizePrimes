#
# This program finds primes!
#     But not particularly well, or anything
# 
# Set this to run on start up, it syncs with git
#     picks up where it left off, and periodically commits new findings
#


echo "Starting PrimeFinder"
git fetch
git pull

$Primes = @()
$LastSave=2

$PRIMESFILE="./CurrentPrimes.txt"
$STARTFILE="./LastSave.txt"

# Get list of found primes
if (Test-Path "$PRIMESFILE") {
  # Watch out, this loads the primes as strings so be mindful of future operations
  $Primes= Get-Content $PRIMESFILE
}

echo ("Loaded " + $Primes.length + " primes")

# Get bookmarked number to start at
if (Test-Path "$STARTFILE") {
  [int]$LastSave= Get-Content $STARTFILE
}

[int]$Current=$LastSave
echo ("Starting at: " + $Current)

$NewPrimes=0
$stopwatch =  [system.diagnostics.stopwatch]::StartNew()
$SaveFreq = 1000

# Begin the hunt!!!
while (1) {
  $isPrime=1

  # Only need to check the first sqrt(n) factors, larger factors must have
  #     a complimentary smaller factor to pair with
  [int]$maxFactor = [math]::Round([math]::Sqrt($Current+1), 0)

  foreach ($prime in $Primes) {
    if (-Not ( $Current % $prime )) {
      $isPrime=0;
      break
    } elseif ( $maxFactor -lt $prime ) {
      break
    }
  }

  # If we found a prime, add it to the file
  if ($isPrime) {
    # Save prime to file, and save checkpoint to avoid duplicating found primes
    $Current.ToString() | Out-File "$PRIMESFILE" -Append -Encoding "UTF8"
    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"
    # Add new prime to our list of primes to check future numbers against
    $Primes+=$Current
    $NewPrimes++
  }

  # Add a save point every 1000 seconds (~16.66 minutes) and commit progress
  if ([math]::Round($stopwatch.Elapsed.TotalSeconds,0) -gt $SaveFreq) {

    # Save progress point to file
    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"

    $CommitMsg = "At $Current, nothing in the last 1000 seconds"
    if ( $NewPrimes -gt 0 ) {
      $rate = [math]::Round(($SaveFreq / $NewPrimes), 3)
      $CommitMsg = ("Found primes up to " + $Primes[-1] + " averaging " + $rate + "sec/prime")
      $NewPrimes=0
    }

    git commit -am "$CommitMsg"
    git push
    $LastSave=$Current
    $stopwatch.Restart()
  }

  $Current++
}
