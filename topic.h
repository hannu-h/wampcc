#ifndef XXX_TOPIC_H
#define XXX_TOPIC_H

#include <jalson/jalson.h>

#include <string>
#include <set>
#include <mutex>
#include <list>
#include <memory>

namespace XXX {

  class wamp_session;
  class topic;
  class dealer_service;

class topic;

class data_model_base
{
public:

  data_model_base(std::string model_type);
  virtual ~data_model_base();

  jalson::json_object & head() { return *m_head; }
  jalson::json_object & body() { return *m_body; }

  void add_publisher(topic*);

protected:

  void apply_model_patch(const jalson::json_array&);

private:
  data_model_base(const data_model_base&) = delete;
  data_model_base& operator=(const data_model_base&) = delete;

  jalson::json_value  m_model;
  jalson::json_object * m_head;
  jalson::json_object * m_body;

  std::vector<topic*> m_publishers;
};


class basic_text_model : public data_model_base
{
public:
  basic_text_model();
  basic_text_model(std::string);

  void set_value(std::string);
  const std::string& get_value() const;

  jalson::json_string * m_value;
};


class topic
{
public:
  topic(std::string uri,
        data_model_base*);

  void add_wamp_session(std::weak_ptr<wamp_session> wp);


  void add_target(std::string realm,
                  dealer_service*);

private:
  void publish_update(const jalson::json_array&);

  std::string m_uri;
  data_model_base * m_model;
  std::vector<std::weak_ptr<wamp_session>> m_sessions;

  std::vector< std::tuple<std::string /* realm */, dealer_service*> > m_dealers;

  friend data_model_base;
};

} // namespace XXX

#endif
