#include "../server/request.h"
#include "../server/response.h"

class GetPostsHandler {
public:
    static void handle_get_posts(Request* req, Response* res);
    static void handle_get_post_details(Request* req, Response* res);
};