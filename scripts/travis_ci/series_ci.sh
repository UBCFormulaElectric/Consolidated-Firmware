curl \
  --header "Authorization: Token 5e287d4c-a391-41e9-b2f5-8cfa38525052" \
  --header "Content-Type: application/json" \
  --data "{\"value\":\"7\",\"sha\":\"${TRAVIS_COMMIT}\"}" \
  https://seriesci.com/api/thekenu/Consolidated-Firmware/:series/one
