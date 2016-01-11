
struct file_path
{
        char* path;
        int on_heap;
};
extern struct file_path get_app_path(char const* expected_path);
extern struct file_path get_app_path_n(char const* buffer, char const* expected_path);
extern void    free_app_path(struct file_path* path);
