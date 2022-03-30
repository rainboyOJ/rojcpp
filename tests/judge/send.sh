curl -v -X POST http://127.0.0.1:8099/handleJudge \
    --cookie cookie \
    --cookie-jar cookie \
   -H 'Content-Type: application/json' \
   --data @judge/send_judge.json
