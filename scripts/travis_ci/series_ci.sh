curl \
  --header "Authorization: Token 92cb0c5d-2b14-41c0-9cfc-06fa089aa108" \
  --header "Content-Type: application/json" \
  --data "{\"values\":[{\"value\":\"9\",\"line\":\"text\"}],\"sha\":\"${TRAVIS_COMMIT}\"}" \
  https://seriesci.com/api/thekenu/Consolidated-Firmware/codesize/many
