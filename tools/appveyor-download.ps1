$apiUrl = 'https://ci.appveyor.com/api'
$token = '6s7v4fnhwpcg0h083d8r'
$headers = @{
  "Authorization" = "Bearer $token"
  "Content-type" = "application/json"
}
$short_header = @{
  "Authorization" = "Bearer $token"
}
$accountName = 'philippkraft'
$projectSlug = 'cmf'

$downloadLocation = 'C:\projects'

# get project with last build details
$project = Invoke-RestMethod -Method Get -Uri "$apiUrl/projects/$accountName/$projectSlug" -Headers $headers

ForEach ($job in $project.build.jobs) {
    # get job artifacts (just to see what we've got)
    $jobId = $job.jobId
    $artifacts = Invoke-RestMethod -Method Get -Uri "$apiUrl/buildjobs/$jobId/artifacts" -Headers $headers

    $artFN = $artifacts[0].fileName -replace "dist/", ""
    $uri = "$apiUrl/buildjobs/$jobId/artifacts/dist/$artFN"
    echo $uri
    Invoke-RestMethod -Method Get -Uri "$uri" -OutFile "$artFN" -Headers $short_header
    echo "Download complete"
}

