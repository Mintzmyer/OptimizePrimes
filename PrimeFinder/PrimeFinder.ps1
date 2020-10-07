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

$NewPrimes=0
$stopwatch =  [system.diagnostics.stopwatch]::StartNew()

# Begin the hunt!!!
while (1) {
  $isPrime=1
  #$maxFactor = [math]::Round([math]::Sqrt($Current+1), 0)
  foreach ($prime in $Primes) {
    #echo ("Current: " + $Current + " | i: " + $prime)
    if ((-Not ( $Current % $prime ))) { # -OR ( $prime -gt $maxFactor)) {
      $isPrime=0;
      continue
    }
  }

  # If we found a prime, add it to the file
  if ($isPrime) {
    $Current.ToString() | Out-File "$PRIMESFILE" -Append -Encoding "UTF8"
    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"
    $Primes+=$Current
    $NewPrimes++
  }

  # Add a save point every 1000 seconds (~16.66 minutes)
  $SaveFreq = 1000
  if ([math]::Round($stopwatch.Elapsed.TotalSeconds,0) -gt $SaveFreq) {

    $Current.ToString() | Out-File "$STARTFILE" -Encoding "UTF8"

    $CommitMsg = "At $Current, nothing in the last 1000 seconds"
    if ( $NewPrimes -gt 0 ) {
      $rate = [math]::Round(($SaveFreq / $NewPrimes), 1)
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
