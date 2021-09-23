#pragma once

namespace mock
{
struct PublicNonDefaultConstructible {
public:
  PublicNonDefaultConstructible(int /*unused*/, float /*unused*/){};
};

struct ProtectedNonDefaultConstructible {
protected:
  ProtectedNonDefaultConstructible(int /*unused*/, float /*unused*/){};
};

struct PublicDefaultConstructible {
public:
  PublicDefaultConstructible() = default;
};

struct ProtectedDefaultConstructible {
protected:
  ProtectedDefaultConstructible() = default;
};
} // namespace mock
