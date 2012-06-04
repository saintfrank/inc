#ifndef HTTP_SERVER3_REQUEST_HANDLER_HPP
#define HTTP_SERVER3_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {
namespace server3 {

struct reply;
struct request;

/// The common handler for all incoming requests.
class Interpreter: private boost::noncopyable {
public:
	/// Construct with a directory containing files to be served.
	explicit Interpreter(const std::string& doc_root);

private:
	/// The directory containing the files to be served.
	std::string request_;


};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_REQUEST_HANDLER_HPP
