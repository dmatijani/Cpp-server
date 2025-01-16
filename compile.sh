#!/bin/bash
# Ako se skripta ne može izvršiti, u korijenskom direktoriju pokrenuti ovo: 'chmod u+x compile.sh'
g++ program.cpp file.cpp server/server.cpp server/request.cpp server/response.cpp html.cpp time.cpp data/objava.cpp data/komentar.cpp handlers/new_post_handler.cpp handlers/get_posts_handler.cpp handlers/get_post_details_handler.cpp handlers/new_comment_handler.cpp -o program
echo "Program izgrađen! Pokreni sa './program [port]', npr. './program 1234'"