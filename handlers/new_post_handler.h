#include "../server/request.h"
#include "../server/response.h"

class NewPostHandler {
public:
    static void handle_new_post(Request* req, Response* res);
};