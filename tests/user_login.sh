curl -v -X POST http://127.0.0.1:8099/user/login \
    --cookie cookie \
    --cookie-jar cookie \
   -H 'Content-Type: application/json' \
   -d '{"username": "test3","password": "password"}'
