#pragma once

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

namespace recipe_9_01 {
  class simple_error : public std::exception {
  public:
    virtual const char* what() const noexcept override
    {
      return "simple exception";
    }
  };

  class another_logic_error : public std::logic_error {
  public:
    another_logic_error()
      : std::logic_error("simple logic exception")
    {
    }
  };

  class advanced_error : public std::runtime_error {
    int error_code;

    std::string make_message(int const e)
    {
      std::stringstream ss;
      ss << "error with code " << e;
      return ss.str();
    }

  public:
    advanced_error(int const e)
      : std::runtime_error(make_message(e).c_str())
      , error_code(e)
    {
    }

    int error() const noexcept
    {
      return error_code;
    }
  };

  void throwing_func()
  {
    throw std::system_error(std::make_error_code(std::errc::timed_out));
  }

  void another_throwing_func()
  {
    throw std::string{ "error!" };
  }

  void throw_simple()
  {
    throw simple_error();
  }

  void throw_advanced()
  {
    throw advanced_error(42);
  }

  void handle_exception()
  {
    try {
      throw; // throw current exception
    } catch (const std::logic_error&) {
    } catch (const std::runtime_error&) {
    } catch (const std::exception&) {
    }
  }

  void execute()
  {
    auto exprint = [](std::exception const& e) { std::cout << e.what() << std::endl; };

    {
      try {
        throwing_func();
      } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
      }
    }

    {
      try {
        throwing_func();
      } catch (std::system_error const& e) {
        exprint(e);
      } catch (std::runtime_error const& e) {
        exprint(e);
      } catch (std::exception const& e) {
        exprint(e);
      } catch (...) {
        std::cout << "unknown exception" << std::endl;
      }
    }

    {
      try {
        try {
          throwing_func();
        } catch (...) {
          auto e = std::current_exception();
          std::rethrow_exception(e);
        }
      } catch (std::exception const& e) {
        exprint(e);
      }
    }

    {
      try {
        throw_simple();
      } catch (simple_error const& e) {
        exprint(e);
      }
    }

    {
      try {
        throw_advanced();
      } catch (advanced_error const& e) {
        exprint(e);
      }
    }

    {
      try {
        throwing_func();
      } catch (...) {
        handle_exception();
      }
    }
  }
}
