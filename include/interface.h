//
// Created by kbratko on 8/16/2023.
//

#ifndef FINAL_INTERFACE_H
#define FINAL_INTERFACE_H

#include <algorithm>
#include <numbers>
#include <string>

#include "micro-engine/micro.h"

#include "metrics.h"
#include "rotation.h"
#include "state.h"
#include "utils.h"

using namespace micro;
using namespace micro::context;

auto interface(State &state) -> void {
    auto const currentTime = ImGui::GetTime();

    if (state.ui.benchmark.automated.enable) {
        if ((state.ui.benchmark.automated.forceRotationsCount &&
             state.ui.benchmark.automated.generatedRotationsCount == state.ui.benchmark.automated.rotationsCount) ||
            (!state.ui.benchmark.automated.forceRotationsCount &&
             state.ui.benchmark.automated.endTime - currentTime <= 0)) {
            state.ui.benchmark.standard.enable  = false;
            state.ui.benchmark.automated.enable = false;

            state.ui.benchmark.standard.totalTime = currentTime - state.ui.benchmark.standard.startTime;

            state.ui.benchmark.averageRotationTime =
                std::accumulate(
                    state.ui.benchmark.metrics.begin(),
                    state.ui.benchmark.metrics.end(),
                    0,
                    [](std::chrono::nanoseconds::rep const &acc, BenchmarkMetric const &bm) { return acc + bm.time; }
                ) /
                static_cast<f64>(state.ui.benchmark.metrics.size());
            auto [min, max] = std::minmax_element(
                state.ui.benchmark.metrics.begin(),
                state.ui.benchmark.metrics.end(),
                [](BenchmarkMetric const &bm1, BenchmarkMetric const &bm2) { return bm1.time < bm2.time; }
            );
            state.ui.benchmark.minRotationTime = min->time;
            state.ui.benchmark.maxRotationTime = max->time;
        }
        else {
            if ((currentTime - state.ui.benchmark.automated.elapsedTime) >=
                static_cast<f64>(state.ui.benchmark.automated.secondsCount) /
                state.ui.benchmark.automated.rotationsCount) {
                state.ui.benchmark.automated.elapsedTime = currentTime;
                state.ui.benchmark.automated.generatedRotationsCount++;

                if (state.ui.rotation.current.mode == RotationMode::Euler) {
                    state.ui.rotation.current.compound = vector3<f32>{
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>,
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>,
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>
                    };
                    state.ui.rotation.modeRotations[state.ui.rotation.current.mode].emplace_back(
                        state.ui.rotation.current);

                    state.ui.rotation.current.compound = vector3<f32>{0.f};

                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
                }
                else {
                    state.ui.rotation.current.simple.angle =
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>;
                    state.ui.rotation.current.simple.axis = vector3<f32>{
                        state.random.distribution(state.random.engine),
                        state.random.distribution(state.random.engine),
                        state.random.distribution(
                            state.random.engine)
                    };
                    state.ui.rotation.modeRotations[state.ui.rotation.current.mode].emplace_back(
                        state.ui.rotation.current);

                    state.ui.rotation.current.simple.angle = 0.f;
                    state.ui.rotation.current.simple.axis  = vector3<f32>{0.f};

                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
                }
            }
        }
    }

    if (state.ui.benchmark.standard.enable) {
        state.ui.benchmark.metrics.emplace_back(
            BenchmarkMetric{
                state.model.vertices.size,
                state.model.indices.size,
                state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size(),
                state.ui.rotation.timeNs
            }
        );
        state.ui.benchmark.standard.framesSinceStartup++;
    }

    if (state.ui.show) {
        ImGui::Begin("Rotation", &state.ui.show, ImGuiWindowFlags_NoTitleBar);

        if (ImGui::CollapsingHeader("Program")) {
            ImGui::BulletText("running on %6.2f fps (frame time: % 8.3f ms)",
                              1 / view.deltaTime, view.deltaTime * 1000.f);
            ImGui::BulletText("time since startup: %.2f s", currentTime);
            ImGui::BulletText("frame count since startup: %zu", ++state.ui.program.framesSinceStartup);
        }

        if (ImGui::CollapsingHeader("Benchmark")) {
            ImGui::BulletText("running for: %.2f s",
                              state.ui.benchmark.standard.enable
                                  ? currentTime - state.ui.benchmark.standard.startTime
                                  : state.ui.benchmark.standard.totalTime);
            ImGui::BulletText("frame count: %zu",
                              state.ui.benchmark.standard.framesSinceStartup);

            ImGui::BeginDisabled(state.ui.benchmark.standard.enable || state.ui.benchmark.automated.enable);
            if (ImGui::Button("Start")) {
                state.ui.benchmark.standard.enable = true;

                state.ui.benchmark.standard.startTime = currentTime;
                state.ui.benchmark.standard.totalTime = 0.f;

                state.ui.benchmark.standard.framesSinceStartup = 0;

                state.ui.benchmark.metrics.clear();
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            if (ImGui::Button("Stop") && state.ui.benchmark.standard.enable) {
                state.ui.benchmark.standard.enable  = false;
                state.ui.benchmark.automated.enable = false;

                state.ui.benchmark.standard.totalTime = currentTime - state.ui.benchmark.standard.startTime;

                state.ui.benchmark.averageRotationTime =
                    std::accumulate(
                        state.ui.benchmark.metrics.begin(),
                        state.ui.benchmark.metrics.end(),
                        0,
                        [](std::chrono::nanoseconds::rep const &acc, BenchmarkMetric const &bm) { return acc + bm.time; }
                    ) /
                    static_cast<f64>(state.ui.benchmark.metrics.size());
                auto [min, max] = std::minmax_element(
                    state.ui.benchmark.metrics.begin(),
                    state.ui.benchmark.metrics.end(),
                    [](BenchmarkMetric const &bm1, BenchmarkMetric const &bm2) { return bm1.time < bm2.time; }
                );
                state.ui.benchmark.minRotationTime = min->time;
                state.ui.benchmark.maxRotationTime = max->time;
            }
            ImGui::SameLine();

            ImGui::BeginDisabled(state.ui.benchmark.standard.enable || state.ui.benchmark.automated.enable);
            if (ImGui::Button("Save")) {
                auto filters = "CSV file (*.csv){.csv}";
                ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose a File", filters, ".");
            }

            if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filename = ImGuiFileDialog::Instance()->GetFilePathName();

                    std::ofstream os{filename};

                    os << "Vertices,Indices,Rotations,Time(ns)\n";
                    std::copy(state.ui.benchmark.metrics.begin(),
                              state.ui.benchmark.metrics.end(),
                              std::ostream_iterator<BenchmarkMetric>(os)
                    );
                }

                ImGuiFileDialog::Instance()->Close();
            }

            ImGui::EndDisabled();

            ImGui::SeparatorText("Automated");

            ImGui::BeginDisabled(state.ui.benchmark.standard.enable || state.ui.benchmark.automated.enable);
            ImGui::InputScalar("Number of rotations", ImGuiDataType_U32, &state.ui.benchmark.automated.rotationsCount);
            ImGui::InputScalar("Number of seconds", ImGuiDataType_U32, &state.ui.benchmark.automated.secondsCount);
            ImGui::Checkbox("Force exact number of rotations", &state.ui.benchmark.automated.forceRotationsCount);
            ImGui::EndDisabled();

            ImGui::BulletText(
                "%u rotations over %u seconds (%.2f minutes) will result in %.3f rotations per second (%.3f seconds per rotation)",
                state.ui.benchmark.automated.rotationsCount, state.ui.benchmark.automated.secondsCount,
                state.ui.benchmark.automated.secondsCount / 60.,
                static_cast<f64>(state.ui.benchmark.automated.rotationsCount) /
                state.ui.benchmark.automated.secondsCount,
                static_cast<f64>(state.ui.benchmark.automated.secondsCount) /
                state.ui.benchmark.automated.rotationsCount
            );

            ImGui::BeginDisabled(state.ui.benchmark.standard.enable || state.ui.benchmark.automated.enable);
            if (ImGui::Button("Start##1")) {
                state.ui.benchmark.standard.enable  = true;
                state.ui.benchmark.automated.enable = true;

                state.ui.benchmark.automated.generatedRotationsCount = 0;

                state.ui.benchmark.automated.endTime = currentTime + state.ui.benchmark.automated.secondsCount;

                state.ui.benchmark.standard.startTime = currentTime;
                state.ui.benchmark.standard.totalTime = 0.f;

                state.ui.benchmark.standard.framesSinceStartup = 0;

                state.ui.benchmark.metrics.clear();
            }
            ImGui::EndDisabled();

            ImGui::SeparatorText("Metrics");

            ImGui::BulletText("average time to calculate rotation matrix: %.2f ns (%.2f us)",
                              state.ui.benchmark.averageRotationTime, state.ui.benchmark.averageRotationTime / 1000.);
            ImGui::BulletText("min time to calculate rotation matrix: %d ns (%.2f us)",
                              state.ui.benchmark.minRotationTime,
                              static_cast<f64>(state.ui.benchmark.minRotationTime) / 1000.);
            ImGui::BulletText("max time to calculate rotation matrix: %d ns (%.2f us)",
                              state.ui.benchmark.maxRotationTime,
                              static_cast<f64>(state.ui.benchmark.maxRotationTime) / 1000.);
        }

        if (ImGui::CollapsingHeader("Model")) {
            ImGui::BulletText("consisting of %d vertices and %d indices",
                              state.model.vertices.size, state.model.indices.size);

            ImGui::BulletText("loaded from file: %s",
                              state.ui.model.path.c_str());

            ImGui::BeginDisabled(state.ui.benchmark.standard.enable || state.ui.benchmark.automated.enable);
            if (ImGui::Button("Load"))
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", state.ui.model.path);

            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    state.ui.model.path = ImGuiFileDialog::Instance()->GetFilePathName();

                    state.ui.model.ready            = false;
                    state.ui.model.boundingBoxReady = false;

                    loadModel(state.ui.model.path.c_str(),
                              state.model, state.boundingBox,
                              [](auto const &msg) { cwarn << msg << std::endl; });

                    state.ui.model.ready            = true;
                    state.ui.model.boundingBoxReady = true;
                }

                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::EndDisabled();

            ImGui::SeparatorText("Settings");

            ImGui::Checkbox("Draw model", &state.ui.model.draw);

            ImGui::Checkbox("Draw bounding box", &state.ui.model.drawBoundingBox);

            ImGui::ColorEdit4("Color", (float *)&state.model.color,
                              ImGuiColorEditFlags_NoInputs);
        }

        if (ImGui::CollapsingHeader("Rotation")) {
            ImGui::BeginDisabled(state.ui.benchmark.automated.enable);
            ImGui::SeparatorText("Mode");
            if (ImGui::RadioButton("Euler", state.ui.rotation.current.mode == RotationMode::Euler) &&
                !state.ui.benchmark.standard.enable) {
                state.ui.rotation.current = Rotation{RotationMode::Euler};

                if (!state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty())
                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Matrix", state.ui.rotation.current.mode == RotationMode::Matrix) &&
                !state.ui.benchmark.standard.enable) {
                state.ui.rotation.current = Rotation{RotationMode::Matrix};

                if (!state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty())
                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Quaternion", state.ui.rotation.current.mode == RotationMode::Quaternion) &&
                !state.ui.benchmark.standard.enable) {
                state.ui.rotation.current = Rotation{RotationMode::Quaternion};

                if (!state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty())
                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
            }

            if (state.ui.rotation.current.mode == RotationMode::Euler) {
                ImGui::SeparatorText("Angles of rotation (XYZ or alpha-beta-gamma gimbal):");

                ImGui::SliderAngle("alpha", &state.ui.rotation.current.compound.x);
                ImGui::SliderAngle("beta", &state.ui.rotation.current.compound.y);
                ImGui::SliderAngle("gamma", &state.ui.rotation.current.compound.z);
            }
            else {
                ImGui::SeparatorText("Angle of rotation");
                ImGui::BulletText("%.2f deg (%.2f rad)",
                                  math::degrees(state.ui.rotation.current.simple.angle),
                                  state.ui.rotation.current.simple.angle);
                ImGui::SliderAngle("angle", &state.ui.rotation.current.simple.angle);

                ImGui::SameLine();

                ImGui::PushButtonRepeat(true);
                if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
                    state.ui.rotation.current.simple.angle > radians(-360.f))
                    state.ui.rotation.current.simple.angle -= radians(90.f);
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                if (ImGui::ArrowButton("##right", ImGuiDir_Right) &&
                    state.ui.rotation.current.simple.angle < radians(360.f))
                    state.ui.rotation.current.simple.angle += radians(90.f);
                ImGui::PopButtonRepeat();

                ImGui::SeparatorText("Axis of rotation");
                auto normalizedAxisOfRotation = math::normalize(state.ui.rotation.current.simple.axis);
                if (state.ui.rotation.current.simple.axis == vector3<f32>{0.f})
                    ImGui::BulletText("[%.3f, %.3f, %.3f]",
                                      state.ui.rotation.current.simple.axis.x, state.ui.rotation.current.simple.axis.y,
                                      state.ui.rotation.current.simple.axis.z);
                else
                    ImGui::BulletText("[%.3f, %.3f, %.3f] (normalized: [%.3f, %.3f, %.3f])",
                                      state.ui.rotation.current.simple.axis.x, state.ui.rotation.current.simple.axis.y,
                                      state.ui.rotation.current.simple.axis.z,
                                      normalizedAxisOfRotation.x, normalizedAxisOfRotation.y,
                                      normalizedAxisOfRotation.z);
                ImGui::SliderFloat("X coordinate", &state.ui.rotation.current.simple.axis.x, -1.f, 1.f);
                if (-0.01f < state.ui.rotation.current.simple.axis.x && state.ui.rotation.current.simple.axis.x < 0.01f)
                    state.ui.rotation.current.simple.axis.x = 0;
                ImGui::SliderFloat("Y coordinate", &state.ui.rotation.current.simple.axis.y, -1.f, 1.f);
                if (-0.01f < state.ui.rotation.current.simple.axis.y && state.ui.rotation.current.simple.axis.y < 0.01f)
                    state.ui.rotation.current.simple.axis.y = 0;
                ImGui::SliderFloat("Z coordinate", &state.ui.rotation.current.simple.axis.z, -1.f, 1.f);
                if (-0.01f < state.ui.rotation.current.simple.axis.z && state.ui.rotation.current.simple.axis.z < 0.01f)
                    state.ui.rotation.current.simple.axis.z = 0;
            }

            ImGui::SeparatorText("Rotation sequence");
            ImGui::BulletText("calculation took: % 7d ns (% 6.2f us)",
                              state.ui.rotation.timeNs, static_cast<f64>(state.ui.rotation.timeNs) / 1000.);

            if (state.ui.rotation.current.mode == RotationMode::Euler) {
                if (ImGui::Button("Random"))
                    state.ui.rotation.current.compound = vector3<f32>{
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>,
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>,
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>
                    };
                ImGui::SameLine();
                if (ImGui::Button("Add") &&
                    state.ui.rotation.current.compound != vector3<f32>{0.f}) {
                    state.ui.rotation.modeRotations[state.ui.rotation.current.mode].emplace_back(
                        state.ui.rotation.current);

                    state.ui.rotation.current.compound = vector3<f32>{0.f};

                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
                }
            }
            else {
                if (ImGui::Button("Random")) {
                    state.ui.rotation.current.simple.angle =
                        state.random.distribution(state.random.engine) * 2 * std::numbers::pi_v<f32>;
                    state.ui.rotation.current.simple.axis = vector3<f32>{
                        state.random.distribution(state.random.engine),
                        state.random.distribution(state.random.engine),
                        state.random.distribution(
                            state.random.engine)
                    };
                }
                ImGui::SameLine();
                if (ImGui::Button("Add") &&
                    state.ui.rotation.current.simple.angle != 0.f &&
                    state.ui.rotation.current.simple.axis != vector3<f32>{0.f}) {
                    state.ui.rotation.modeRotations[state.ui.rotation.current.mode].emplace_back(
                        state.ui.rotation.current);

                    state.ui.rotation.current.simple.angle = 0.f;
                    state.ui.rotation.current.simple.axis  = vector3<f32>{0.f};

                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Pop") && !state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty()) {
                state.ui.rotation.modeRotations[state.ui.rotation.current.mode].pop_back();

                if (!state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty())
                    state.ui.rotation.modeRotationIndex =
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size() - 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear") && !state.ui.rotation.modeRotations[state.ui.rotation.current.mode].empty()) {
                state.ui.rotation.modeRotations[state.ui.rotation.current.mode].clear();

                state.ui.rotation.modeRotationIndex = 0;
            }
            ImGui::EndDisabled();

            {
                char text[32];
                sprintf(text, "Rotations (%zu)",
                        state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size());
                ImGui::SeparatorText(text);
            }

            ImGui::Checkbox("Show", &state.ui.rotation.show);

            if (state.ui.rotation.show) {
                ImGui::BeginChild("Scrolling");
                for (auto i = 0; i < state.ui.rotation.modeRotations[state.ui.rotation.current.mode].size(); ++i) {
                    char text[256];
                    sprintf(text, "R%d - %s", i,
                            state.ui.rotation.modeRotations[state.ui.rotation.current.mode].at(i).toString().c_str());
                    if (ImGui::Selectable(text, state.ui.rotation.modeRotationIndex == i))
                        state.ui.rotation.modeRotationIndex = i;
                }
                ImGui::EndChild();
            }
        }

        ImGui::End();
    }
}

#endif //FINAL_INTERFACE_H
