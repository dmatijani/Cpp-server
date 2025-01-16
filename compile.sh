#!/bin/bash
# Ako se skripta ne može izvršiti, u korijenskom direktoriju pokrenuti ovo: 'chmod u+x compile.sh'
g++ program.cpp file.cpp server/server.cpp server/request.cpp server/response.cpp html.cpp data/objava.cpp handlers/new_post_handler.cpp -o program
echo "Program izgrađen! Pokreni sa './program [port]', npr. './program 1234'"