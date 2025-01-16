#include "../server/request.h"
#include "../server/response.h"

class GetPostsHandler {
public:
    static void handle_get_posts(Request* req, Response* res);
};