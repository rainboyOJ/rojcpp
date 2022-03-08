curl -v -X POST http://127.0.0.1:8099/user/register \
   -H 'Content-Type: application/json' \
   -d '{"username": "test2" , "nickname": "test1_nickname" ,"password": "password", "school":   "fake school", "email":    "fake1@qq.mail"}'
