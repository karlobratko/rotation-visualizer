//
// Created by kbratko on 8/16/2023.
//

#ifndef FINAL_STATE_H
#define FINAL_STATE_H

#include <random>

#include "micro-engine/micro.h"

#include "constants.h"
#include "metrics.h"
#include "rotation.h"

using namespace micro;
using namespace micro::core;
using namespace micro::math;
using namespace micro::graphics;

auto printAndExit(std::string const &message) -> void;

struct ModelState {
    Shader       shader;
    Buffer       vertices{Buffer::Target::Vertex};
    Buffer       indices{Buffer::Target::Index};
    Model        model{};
    vector4<f32> color{1.f};

    ModelState() : shader{
        graphics::shader(
            {
                {Shader::Type::Vertex, Application::Shaders::Vertex::Model},
                {Shader::Type::Fragment, Application::Shaders::Fragment::Model}
            },
            printAndExit
        )
    } {}
};

struct BoundingBoxState {
    BoundingBox box{};
    Shader      shader{};
    Buffer      vertices{Buffer::Target::Vertex};
    Buffer      indices{Buffer::Target::Index};
    Model       model{};

    BoundingBoxState() : shader{
        graphics::shader(
            {
                {Shader::Type::Vertex, Application::Shaders::Vertex::BoundingBox},
                {Shader::Type::Fragment, Application::Shaders::Fragment::BoundingBox}
            },
            printAndExit
        )
    } {}
};

struct State {
    struct UiState {
        bool show = true;

        struct ProgramSectionState {
            usize framesSinceStartup = 0;
        }         program;

        struct BenchmarkSectionState {
            struct StandardBenchmarkState {
                bool enable = false;

                f64   startTime          = 0.;
                f64   totalTime          = 0.;
                usize framesSinceStartup = 0;
            }         standard;

            struct AutomatedBenchmarkState {
                bool enable = false;

                f64 elapsedTime = 0.;
                f64 endTime     = 0.;

                u32  rotationsCount          = 1;
                u32  secondsCount            = 1;
                bool forceRotationsCount     = false;
                u32  generatedRotationsCount = 0;
            }        automated;

            std::vector<BenchmarkMetric> metrics{};

            f64                           averageRotationTime = 0.;
            std::chrono::nanoseconds::rep minRotationTime     = 0;
            std::chrono::nanoseconds::rep maxRotationTime     = 0;

            BenchmarkSectionState() { metrics.reserve(1'000'000); }
        } benchmark;

        struct ModelSectionState {
            bool ready            = false;
            bool boundingBoxReady = false;

            bool draw            = true;
            bool drawBoundingBox = false;

            std::string path;
        }               model;

        struct RotationSectionState {
            bool                          show   = true;
            std::chrono::nanoseconds::rep timeNs = 0;

            Rotation current = Rotation{RotationMode::Quaternion};

            usize                                         modeRotationIndex = 0;
            std::map<RotationMode, std::vector<Rotation>> modeRotations{};

            RotationSectionState() {
                auto vec = std::vector<Rotation>{};
                vec.reserve(105'000);
                modeRotations = std::map<RotationMode, std::vector<Rotation>>{
                    {RotationMode::Euler, vec},
                    {RotationMode::Matrix, vec},
                    {RotationMode::Quaternion, vec}
                };
            }
        } rotation;
    }     ui;

    struct RandomState {
        std::default_random_engine          engine;
        std::uniform_real_distribution<f32> distribution;

        RandomState()
            : engine{
                  std::default_random_engine{static_cast<u32>(std::chrono::system_clock::now().time_since_epoch().count())}
              },
              distribution{
                  std::uniform_real_distribution<f32>{-1.f, std::nextafter(1.f, std::numeric_limits<f32>::max())}
              } {}
    } random;

    ModelState model;

    BoundingBoxState boundingBox;
};

#endif //FINAL_STATE_H
