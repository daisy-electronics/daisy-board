namespace SetRelay {
  extern const char *request_subject;

  void setup();
  void do_sync(uint32_t request_id, const char *message);
};
