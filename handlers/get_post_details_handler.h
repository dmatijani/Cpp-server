#include "../server/request.h"
#include "../server/response.h"

class GetPostDetailsHandler {
public:
    static void handle_get_post_details(Request* req, Response* res);
};