load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

package(
    default_visibility = ["//visibility:public"],
)

proto_library(
    name = "feature_proto",
    srcs = ["tensorflow/core/example/feature.proto"],
)

proto_library(
    name = "example_proto",
    srcs = ["tensorflow/core/example/example.proto"],
    deps = [":feature_proto"],
)

proto_library(
    name = "types_proto",
    srcs = ["tensorflow/core/framework/types.proto"],
)

proto_library(
    name = "versions_proto",
    srcs = ["tensorflow/core/framework/versions.proto"],
)

proto_library(
    name = "tensor_shape_proto",
    srcs = ["tensorflow/core/framework/tensor_shape.proto"],
)

proto_library(
    name = "resource_handle_proto",
    srcs = ["tensorflow/core/framework/resource_handle.proto"],
)

proto_library(
    name = "tensor_proto",
    srcs = ["tensorflow/core/framework/tensor.proto"],
    deps = [
        ":resource_handle_proto",
        ":tensor_shape_proto",
        ":types_proto",
    ],
)

proto_library(
    name = "attr_value_proto",
    srcs = ["tensorflow/core/framework/attr_value.proto"],
    deps = [
        ":tensor_proto",
        ":tensor_shape_proto",
        ":types_proto",
    ],
)

proto_library(
    name = "node_def_proto",
    srcs = ["tensorflow/core/framework/node_def.proto"],
    deps = [
        ":attr_value_proto",
    ],
)

proto_library(
    name = "op_def_proto",
    srcs = ["tensorflow/core/framework/op_def.proto"],
    deps = [
        ":attr_value_proto",
        ":types_proto",
    ],
)

proto_library(
    name = "function_proto",
    srcs = ["tensorflow/core/framework/function.proto"],
    deps = [
        ":attr_value_proto",
        ":node_def_proto",
        ":op_def_proto",
    ],
)

proto_library(
    name = "graph_proto",
    srcs = ["tensorflow/core/framework/graph.proto"],
    deps = [
        ":function_proto",
        ":node_def_proto",
        ":versions_proto",
    ],
)

proto_library(
    name = "saver_proto",
    srcs = ["tensorflow/core/protobuf/saver.proto"],
)

proto_library(
    name = "meta_graph_proto",
    srcs = ["tensorflow/core/protobuf/meta_graph.proto"],
    deps = [
        ":graph_proto",
        ":op_def_proto",
        ":saver_proto",
        ":tensor_shape_proto",
        ":types_proto",
        "@com_google_protobuf//:any_proto",
    ],
)

proto_library(
    name = "input_proto",
    srcs = ["tensorflow_serving/apis/input.proto"],
    deps = [
        ":example_proto",
    ],
)

proto_library(
    name = "model_proto",
    srcs = ["tensorflow_serving/apis/model.proto"],
    deps = [
        "@com_google_protobuf//:wrappers_proto",
    ],
)

proto_library(
    name = "classification_proto",
    srcs = ["tensorflow_serving/apis/classification.proto"],
    deps = [
        ":input_proto",
        ":model_proto",
    ],
)

proto_library(
    name = "regression_proto",
    srcs = ["tensorflow_serving/apis/regression.proto"],
    deps = [
        ":input_proto",
        ":model_proto",
    ],
)

proto_library(
    name = "get_model_metadata_proto",
    srcs = ["tensorflow_serving/apis/get_model_metadata.proto"],
    deps = [
        ":meta_graph_proto",
        ":model_proto",
        "@com_google_protobuf//:any_proto",
    ],
)

proto_library(
    name = "inference_proto",
    srcs = ["tensorflow_serving/apis/inference.proto"],
    deps = [
        ":classification_proto",
        ":input_proto",
        ":model_proto",
        ":regression_proto",
    ],
)

proto_library(
    name = "predict_proto",
    srcs = ["tensorflow_serving/apis/predict.proto"],
    deps = [
        ":model_proto",
        ":tensor_proto",
    ],
)

proto_library(
    name = "prediction_service_proto",
    srcs = ["tensorflow_serving/apis/prediction_service.proto"],
    deps = [
        ":classification_proto",
        ":get_model_metadata_proto",
        ":inference_proto",
        ":predict_proto",
        ":regression_proto",
    ],
)

cc_proto_library(
    name = "prediction_service_cc_proto",
    deps = [":prediction_service_proto"],
)

cc_grpc_library(
    name = "prediction_service_cc_grpc",
    srcs = [":prediction_service_proto"],
    grpc_only = True,
    deps = [":prediction_service_cc_proto"],
)


cc_library(
    name = "main_lib",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":prediction_service_cc_grpc",
        ":prediction_service_cc_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
