#include "create_request.h"

Createrequest::Createrequest(Filename &name, int type, int storage_class,
                             int location_class, int enumerable)
    : NamenodeRequest(static_cast<short>(RpcCommand::Create),
                      static_cast<short>(RequestType::Create)),
      filename_(name), type_(type), storage_class_(storage_class),
      location_class_(location_class), enumerable_(enumerable) {
  this->filename_ = std::move(name);
}

Createrequest::~Createrequest() {}

int Createrequest::Write(ByteBuffer &buf) const {
  NamenodeRequest::Write(buf);

  filename_.Write(buf);
  buf.PutInt(type_);
  buf.PutInt(storage_class_);
  buf.PutInt(location_class_);
  buf.PutInt(enumerable_);

  return Size();
}

int Createrequest::Update(ByteBuffer &buf) {
  NamenodeRequest::Update(buf);

  filename_.Update(buf);
  type_ = buf.GetInt();
  storage_class_ = buf.GetInt();
  location_class_ = buf.GetInt();
  enumerable_ = buf.GetInt();

  return Size();
}
