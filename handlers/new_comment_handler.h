#include "../server/request.h"
#include "../server/response.h"

class NewCommentHandler {
public:
    static void handle_new_comment(Request* req, Response* res);
};