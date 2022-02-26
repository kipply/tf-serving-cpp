
#include "grpcpp/grpcpp.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

int main(int argc, char** argv) {
  const auto& prediction_service_stub =
      tensorflow::serving::PredictionService::NewStub(grpc::CreateChannel(
          "localhost:9000", grpc::InsecureChannelCredentials()));

  grpc::ClientContext client_context;

  //
  // use GetModelMetadata to validate that the model name exists and has the
  // expected signatures
  //
  tensorflow::serving::GetModelMetadataRequest get_model_metadata_request;
  tensorflow::serving::ModelSpec* model_spec =
      get_model_metadata_request.mutable_model_spec();
  model_spec->set_name("model_name");  // specify your model name here
  get_model_metadata_request.add_metadata_field(
      "signature_def");  // this is actually the only valid string for this
                         // field

  tensorflow::serving::GetModelMetadataResponse get_model_metadata_response;

  const grpc::Status status = prediction_service_stub->GetModelMetadata(
      &client_context, get_model_metadata_request,
      &get_model_metadata_response);
  if (!status.ok()) {
    std::cerr << status.error_message() << std::endl;
    return -1;
  }
  const auto& metadata = get_model_metadata_response.metadata();

  const auto& sig_msg = metadata.at("signature_def");
  tensorflow::serving::SignatureDefMap sig_def;
  sig_msg.UnpackTo(&sig_def);
  const auto& sig_map = sig_def.signature_def();

  if (sig_map.find("some_signature") ==
      sig_map.end()) {  // specify your signature name here
    std::cerr << "model appears to be missing the signature" << std::endl;
    return -1;
  }

  //
  // make a predict request
  //

  tensorflow::serving::PredictRequest predict_request;
  tensorflow::serving::PredictResponse predict_response;
  grpc::ClientContext cli_context;

  predict_request.mutable_model_spec()->set_name(
      "model_name");  // specify model name again
  predict_request.mutable_model_spec()->set_signature_name(
      "some_signature");  // specify signature again
  google::protobuf::Map<std::string, tensorflow::TensorProto>& inputs =
      *predict_request.mutable_inputs();

  tensorflow::TensorProto input_tensor;
  input_tensor.set_dtype(tensorflow::DataType::DT_INT32);
  input_tensor.mutable_tensor_shape()->add_dim()->set_size(2);
  input_tensor.add_int_val(1);
  input_tensor.add_int_val(2);
  inputs.insert(
      {"input_key",
       input_tensor});  // specify your input keys and repeat as necessary

  const grpc::Status& predict_status = prediction_service_stub->Predict(
      &cli_context, predict_request, &predict_response);

  if (!predict_status.ok()) {
    std::cerr << predict_status.error_message() << std::endl;
    return -1;
  }

  for (const auto& output_pair : predict_response.outputs()) {
    std::cout << "Output " << output_pair.first << std::endl;
    auto tensor = output_pair.second;

    for (const auto val : tensor.int_val()) {
      std::cout << "\t" << val;
    }
    std::cout << std::endl;
  }

  return 0;
}
