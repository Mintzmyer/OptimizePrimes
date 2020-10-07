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
  $Primes= Get-Content $PRIMESFILE
}

echo ("Loaded " + $Primes.length + " primes")

# Get bookmarked number to start at
if (Test-Path "$STARTFILE") {
  [int]$LastSave= Get-Content $STARTFILE
}

[int]$Current=$LastSave
echo ("Starting at: " + $Current)

$FoundPrimesSinceSave=0
$stopwatch =  [system.diagnostics.stopwatch]::StartNew()

# Begin the hunt!!!
while (1) {
  $isPrime=1
  foreach ($prime in $Primes) {
    #echo ("Current: " + $Current + " | i: " + $prime)
    if (-Not ( $Current % $prime )) {
      $isPrime=0;
      continue
    }
  }

  # If we found a prime, add it to the file
  if ($isPrime) {
    $Current.ToString() | Out-File "$PRIMESFILE" -Append -Encoding "UTF8"
    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"
    $Primes+=$Current
    $FoundPrimesSinceSave=1
  }

  # Add a save point every 1000 seconds (~16.66 minutes)
  if ([math]::Round($stopwatch.Elapsed.TotalSeconds,0) -gt 1000) {

    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"

    $CommitMsg = "At $Current, nothing in the last 1000 seconds"
    if ( $FoundPrimesSinceSave ) {
      $CommitMsg = ("Found primes up to " + $Primes[-1])
      $FoundPrimesSinceSave=0
    }

    git commit -am "$CommitMsg"
    git push
    $LastSave=$Current
    $stopwatch.Restart()
  }

  $Current++
}
