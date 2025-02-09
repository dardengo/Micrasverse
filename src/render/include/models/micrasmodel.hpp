#ifndef MICRASMODEL_HPP
#define MICRASMODEL_HPP

#include "render/model.hpp"
#include "config/constants.hpp"

namespace micrasverse::render {

class MicrasModel : public Model {
public:

    MicrasModel(glm::vec3 position = glm::vec3((CELL_SIZE + WALL_THICKNESS)/ 2.0f, (CELL_SIZE + WALL_THICKNESS)/ 2.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
        : Model(position, size) {}

    void init() {
        int noVertices = 0;

        float vertices[] = {
        //Positions         
        0.034,-0.0632,-0.0161,
        0.034,-0.0632,-0.0177,
        0.034,-0.0442,-0.0161,
        0.034,-0.0442,-0.0161,
        0.034,-0.0632,-0.0177,
        0.034,-0.0442,-0.0177,
        -0.0175,-0.0162,-0.0177,
        -0.0175,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0175,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0161,
        0.0185,-0.0162,-0.0177,
        0.0185,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0161,
        0.0185,-0.0162,-0.0161,
        0.0185,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        0.034,-0.0442,-0.0161,
        0.034,-0.0442,-0.0161,
        -0.0112,-0.0162,-0.0161,
        0.022,-0.0632,-0.0161,
        0.034,-0.0442,-0.0161,
        0.022,-0.0632,-0.0161,
        0.034,-0.0632,-0.0161,
        0.0184,-0.081,-0.0161,
        0.022,-0.0905,-0.0161,
        0.0186,-0.0805,-0.0161,
        0.0186,-0.0805,-0.0161,
        0.022,-0.0905,-0.0161,
        0.022,-0.0632,-0.0161,
        0.0186,-0.0805,-0.0161,
        0.022,-0.0632,-0.0161,
        0.0184,-0.0799,-0.0161,
        0.0184,-0.081,-0.0161,
        0.018,-0.0814,-0.0161,
        0.022,-0.0905,-0.0161,
        0.022,-0.0905,-0.0161,
        0.018,-0.0814,-0.0161,
        0.0175,-0.0815,-0.0161,
        0.022,-0.0905,-0.0161,
        0.0175,-0.0815,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.0175,-0.0815,-0.0161,
        0.017,-0.0814,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.017,-0.0814,-0.0161,
        0.0166,-0.081,-0.0161,
        0.0077,-0.0621,-0.0161,
        0.008,-0.0649,-0.0161,
        0.0165,-0.0805,-0.0161,
        0.0165,-0.0805,-0.0161,
        0.008,-0.0649,-0.0161,
        0.0072,-0.0675,-0.0161,
        0.0165,-0.0805,-0.0161,
        0.0072,-0.0675,-0.0161,
        0.0166,-0.081,-0.0161,
        -0.0069,-0.0649,-0.0161,
        -0.0067,-0.0621,-0.0161,
        -0.0112,-0.0162,-0.0161,
        0.0005,-0.0717,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.0032,-0.0712,-0.0161,
        0.0032,-0.0712,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.0166,-0.081,-0.0161,
        0.0032,-0.0712,-0.0161,
        0.0166,-0.081,-0.0161,
        0.0056,-0.0697,-0.0161,
        0.0056,-0.0697,-0.0161,
        0.0166,-0.081,-0.0161,
        0.0072,-0.0675,-0.0161,
        0.0005,-0.0717,-0.0161,
        -0.0022,-0.0712,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0022,-0.0712,-0.0161,
        -0.0045,-0.0697,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0045,-0.0697,-0.0161,
        -0.0062,-0.0675,-0.0161,
        -0.0067,-0.0621,-0.0161,
        -0.0055,-0.0597,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0055,-0.0597,-0.0161,
        -0.0034,-0.0578,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0034,-0.0578,-0.0161,
        -0.0009,-0.0568,-0.0161,
        -0.0009,-0.0568,-0.0161,
        0.0019,-0.0568,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        0.0019,-0.0568,-0.0161,
        0.0045,-0.0578,-0.0161,
        -0.0112,-0.0162,-0.0161,
        0.0045,-0.0578,-0.0161,
        0.022,-0.0632,-0.0161,
        0.022,-0.0632,-0.0161,
        0.0045,-0.0578,-0.0161,
        0.0065,-0.0597,-0.0161,
        0.0165,-0.0805,-0.0161,
        0.0166,-0.0799,-0.0161,
        0.0077,-0.0621,-0.0161,
        0.0077,-0.0621,-0.0161,
        0.0166,-0.0799,-0.0161,
        0.017,-0.0795,-0.0161,
        0.0077,-0.0621,-0.0161,
        0.017,-0.0795,-0.0161,
        0.0065,-0.0597,-0.0161,
        0.0065,-0.0597,-0.0161,
        0.017,-0.0795,-0.0161,
        0.0175,-0.0794,-0.0161,
        0.0065,-0.0597,-0.0161,
        0.0175,-0.0794,-0.0161,
        0.022,-0.0632,-0.0161,
        0.022,-0.0632,-0.0161,
        0.0175,-0.0794,-0.0161,
        0.018,-0.0795,-0.0161,
        0.022,-0.0632,-0.0161,
        0.018,-0.0795,-0.0161,
        0.0184,-0.0799,-0.0161,
        0.034,-0.1162,-0.0161,
        0.034,-0.1032,-0.0161,
        0.022,-0.1032,-0.0161,
        0.022,-0.0905,-0.0161,
        0.018,-0.0995,-0.0161,
        0.022,-0.1032,-0.0161,
        0.022,-0.1032,-0.0161,
        0.018,-0.0995,-0.0161,
        0.0184,-0.0999,-0.0161,
        0.022,-0.1032,-0.0161,
        0.0184,-0.0999,-0.0161,
        0.034,-0.1162,-0.0161,
        0.034,-0.1162,-0.0161,
        0.0184,-0.0999,-0.0161,
        0.0186,-0.1005,-0.0161,
        0.034,-0.1162,-0.0161,
        0.0186,-0.1005,-0.0161,
        0.0184,-0.101,-0.0161,
        0.0184,-0.101,-0.0161,
        0.018,-0.1014,-0.0161,
        0.034,-0.1162,-0.0161,
        0.034,-0.1162,-0.0161,
        0.018,-0.1014,-0.0161,
        0.0175,-0.1015,-0.0161,
        0.034,-0.1162,-0.0161,
        0.0175,-0.1015,-0.0161,
        -0.0112,-0.1162,-0.0161,
        0.0175,-0.1015,-0.0161,
        0.017,-0.1014,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0161,
        0.017,-0.1014,-0.0161,
        0.0166,-0.101,-0.0161,
        -0.0112,-0.1162,-0.0161,
        0.0166,-0.101,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.0166,-0.101,-0.0161,
        0.0165,-0.1005,-0.0161,
        0.0165,-0.1005,-0.0161,
        0.0166,-0.0999,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.0166,-0.0999,-0.0161,
        0.017,-0.0995,-0.0161,
        -0.0112,-0.0905,-0.0161,
        0.017,-0.0995,-0.0161,
        0.022,-0.0905,-0.0161,
        0.022,-0.0905,-0.0161,
        0.017,-0.0995,-0.0161,
        0.0175,-0.0994,-0.0161,
        0.022,-0.0905,-0.0161,
        0.0175,-0.0994,-0.0161,
        0.018,-0.0995,-0.0161,
        -0.033,-0.1032,-0.0161,
        -0.033,-0.1162,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.033,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.017,-0.1014,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.0165,-0.1015,-0.0161,
        -0.0165,-0.1015,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0165,-0.1015,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.016,-0.1014,-0.0161,
        -0.0174,-0.0999,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.0175,-0.1005,-0.0161,
        -0.0175,-0.1005,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.0175,-0.1005,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.0174,-0.101,-0.0161,
        -0.0174,-0.101,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.017,-0.1014,-0.0161,
        -0.0174,-0.0999,-0.0161,
        -0.017,-0.0995,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.017,-0.0995,-0.0161,
        -0.0165,-0.0994,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.0165,-0.0994,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0165,-0.0994,-0.0161,
        -0.016,-0.0995,-0.0161,
        -0.016,-0.0995,-0.0161,
        -0.0156,-0.0999,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0156,-0.0999,-0.0161,
        -0.0154,-0.1005,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0154,-0.1005,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0154,-0.1005,-0.0161,
        -0.0156,-0.101,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0156,-0.101,-0.0161,
        -0.016,-0.1014,-0.0161,
        -0.033,-0.0632,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.033,-0.0442,-0.0161,
        -0.033,-0.0442,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.033,-0.0442,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0175,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.016,-0.0795,-0.0161,
        -0.0156,-0.0799,-0.0161,
        -0.0062,-0.0675,-0.0161,
        -0.0069,-0.0649,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0069,-0.0649,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0154,-0.0805,-0.0161,
        -0.0154,-0.0805,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0156,-0.0799,-0.0161,
        -0.017,-0.0814,-0.0161,
        -0.0174,-0.081,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.0174,-0.081,-0.0161,
        -0.0175,-0.0805,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.0175,-0.0805,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.0175,-0.0805,-0.0161,
        -0.0174,-0.0799,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.0174,-0.0799,-0.0161,
        -0.017,-0.0795,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.017,-0.0795,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.017,-0.0795,-0.0161,
        -0.0165,-0.0794,-0.0161,
        -0.0112,-0.0162,-0.0161,
        -0.0165,-0.0794,-0.0161,
        -0.016,-0.0795,-0.0161,
        -0.0154,-0.0805,-0.0161,
        -0.0156,-0.081,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.0156,-0.081,-0.0161,
        -0.016,-0.0814,-0.0161,
        -0.0112,-0.0905,-0.0161,
        -0.016,-0.0814,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.016,-0.0814,-0.0161,
        -0.0165,-0.0815,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.0165,-0.0815,-0.0161,
        -0.017,-0.0814,-0.0161,
        -0.033,-0.0632,-0.0177,
        -0.033,-0.0442,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.033,-0.0442,-0.0177,
        -0.0175,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.016,-0.0795,-0.0177,
        -0.0175,-0.0162,-0.0177,
        -0.0175,-0.0162,-0.0177,
        -0.016,-0.0795,-0.0177,
        -0.0165,-0.0794,-0.0177,
        -0.0175,-0.0162,-0.0177,
        -0.0165,-0.0794,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.0165,-0.0794,-0.0177,
        -0.017,-0.0795,-0.0177,
        -0.017,-0.0795,-0.0177,
        -0.0174,-0.0799,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.0174,-0.0799,-0.0177,
        -0.0175,-0.0805,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.0175,-0.0805,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.0175,-0.0805,-0.0177,
        -0.0174,-0.081,-0.0177,
        -0.0174,-0.081,-0.0177,
        -0.017,-0.0814,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.017,-0.0814,-0.0177,
        -0.0165,-0.0815,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.0165,-0.0815,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0165,-0.0815,-0.0177,
        -0.016,-0.0814,-0.0177,
        -0.016,-0.0814,-0.0177,
        -0.0156,-0.081,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0156,-0.081,-0.0177,
        -0.0154,-0.0805,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0154,-0.0805,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0154,-0.0805,-0.0177,
        -0.0156,-0.0799,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0156,-0.0799,-0.0177,
        -0.016,-0.0795,-0.0177,
        -0.033,-0.1162,-0.0177,
        -0.033,-0.1032,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.033,-0.1032,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.0175,-0.1005,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.0174,-0.0999,-0.0177,
        -0.0174,-0.0999,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.0174,-0.0999,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.017,-0.0995,-0.0177,
        -0.016,-0.1014,-0.0177,
        -0.0156,-0.101,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0156,-0.101,-0.0177,
        -0.0154,-0.1005,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0154,-0.1005,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0154,-0.1005,-0.0177,
        -0.0156,-0.0999,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0156,-0.0999,-0.0177,
        -0.016,-0.0995,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.016,-0.0995,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.016,-0.0995,-0.0177,
        -0.0165,-0.0994,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.0165,-0.0994,-0.0177,
        -0.017,-0.0995,-0.0177,
        -0.0175,-0.1005,-0.0177,
        -0.0174,-0.101,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.0174,-0.101,-0.0177,
        -0.017,-0.1014,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.017,-0.1014,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.017,-0.1014,-0.0177,
        -0.0165,-0.1015,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0165,-0.1015,-0.0177,
        -0.016,-0.1014,-0.0177,
        0.034,-0.1032,-0.0177,
        0.034,-0.1162,-0.0177,
        0.022,-0.1032,-0.0177,
        0.022,-0.1032,-0.0177,
        0.034,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        0.0175,-0.0994,-0.0177,
        0.017,-0.0995,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.0175,-0.0994,-0.0177,
        0.022,-0.0905,-0.0177,
        0.018,-0.0995,-0.0177,
        0.018,-0.0995,-0.0177,
        0.022,-0.0905,-0.0177,
        0.0184,-0.0999,-0.0177,
        0.0166,-0.101,-0.0177,
        -0.0112,-0.1162,-0.0177,
        0.0165,-0.1005,-0.0177,
        0.0165,-0.1005,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.0165,-0.1005,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.0166,-0.0999,-0.0177,
        0.0166,-0.0999,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.017,-0.0995,-0.0177,
        0.0184,-0.0999,-0.0177,
        0.022,-0.0905,-0.0177,
        0.0186,-0.1005,-0.0177,
        0.0186,-0.1005,-0.0177,
        0.022,-0.0905,-0.0177,
        0.022,-0.1032,-0.0177,
        0.0186,-0.1005,-0.0177,
        0.022,-0.1032,-0.0177,
        0.0184,-0.101,-0.0177,
        0.0166,-0.101,-0.0177,
        0.017,-0.1014,-0.0177,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        0.017,-0.1014,-0.0177,
        0.0175,-0.1015,-0.0177,
        -0.0112,-0.1162,-0.0177,
        0.0175,-0.1015,-0.0177,
        0.022,-0.1032,-0.0177,
        0.022,-0.1032,-0.0177,
        0.0175,-0.1015,-0.0177,
        0.018,-0.1014,-0.0177,
        0.022,-0.1032,-0.0177,
        0.018,-0.1014,-0.0177,
        0.0184,-0.101,-0.0177,
        0.034,-0.0442,-0.0177,
        0.034,-0.0632,-0.0177,
        0.0185,-0.0162,-0.0177,
        0.0185,-0.0162,-0.0177,
        0.034,-0.0632,-0.0177,
        0.022,-0.0632,-0.0177,
        0.017,-0.0795,-0.0177,
        -0.0022,-0.0712,-0.0177,
        0.0175,-0.0794,-0.0177,
        0.0175,-0.0794,-0.0177,
        -0.0022,-0.0712,-0.0177,
        0.0005,-0.0717,-0.0177,
        0.022,-0.0905,-0.0177,
        0.0175,-0.0815,-0.0177,
        0.018,-0.0814,-0.0177,
        0.0175,-0.0994,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.022,-0.0905,-0.0177,
        0.022,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.017,-0.0814,-0.0177,
        0.022,-0.0905,-0.0177,
        0.017,-0.0814,-0.0177,
        0.0175,-0.0815,-0.0177,
        0.018,-0.0814,-0.0177,
        0.0184,-0.081,-0.0177,
        0.022,-0.0905,-0.0177,
        0.022,-0.0905,-0.0177,
        0.0184,-0.081,-0.0177,
        0.0186,-0.0805,-0.0177,
        0.022,-0.0905,-0.0177,
        0.0186,-0.0805,-0.0177,
        0.022,-0.0632,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0186,-0.0805,-0.0177,
        0.0184,-0.0799,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0184,-0.0799,-0.0177,
        0.018,-0.0795,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0077,-0.0621,-0.0177,
        0.0065,-0.0597,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0045,-0.0697,-0.0177,
        -0.0022,-0.0712,-0.0177,
        0.0005,-0.0717,-0.0177,
        0.0032,-0.0712,-0.0177,
        0.0175,-0.0794,-0.0177,
        0.0175,-0.0794,-0.0177,
        0.0032,-0.0712,-0.0177,
        0.0056,-0.0697,-0.0177,
        0.0175,-0.0794,-0.0177,
        0.0056,-0.0697,-0.0177,
        0.018,-0.0795,-0.0177,
        0.018,-0.0795,-0.0177,
        0.0056,-0.0697,-0.0177,
        0.0072,-0.0675,-0.0177,
        0.018,-0.0795,-0.0177,
        0.0072,-0.0675,-0.0177,
        0.022,-0.0632,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0072,-0.0675,-0.0177,
        0.008,-0.0649,-0.0177,
        0.022,-0.0632,-0.0177,
        0.008,-0.0649,-0.0177,
        0.0077,-0.0621,-0.0177,
        -0.0112,-0.0162,-0.0177,
        0.0185,-0.0162,-0.0177,
        0.0019,-0.0568,-0.0177,
        0.0019,-0.0568,-0.0177,
        0.0185,-0.0162,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0019,-0.0568,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0045,-0.0578,-0.0177,
        0.0045,-0.0578,-0.0177,
        0.022,-0.0632,-0.0177,
        0.0065,-0.0597,-0.0177,
        0.0019,-0.0568,-0.0177,
        -0.0009,-0.0568,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0009,-0.0568,-0.0177,
        -0.0034,-0.0578,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0034,-0.0578,-0.0177,
        -0.0055,-0.0597,-0.0177,
        -0.0055,-0.0597,-0.0177,
        -0.0067,-0.0621,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0067,-0.0621,-0.0177,
        -0.0069,-0.0649,-0.0177,
        -0.0112,-0.0162,-0.0177,
        -0.0069,-0.0649,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0069,-0.0649,-0.0177,
        -0.0062,-0.0675,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0062,-0.0675,-0.0177,
        -0.0045,-0.0697,-0.0177,
        0.017,-0.0795,-0.0177,
        0.0166,-0.0799,-0.0177,
        -0.0022,-0.0712,-0.0177,
        -0.0022,-0.0712,-0.0177,
        0.0166,-0.0799,-0.0177,
        0.0165,-0.0805,-0.0177,
        -0.0022,-0.0712,-0.0177,
        0.0165,-0.0805,-0.0177,
        -0.0112,-0.0905,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.0165,-0.0805,-0.0177,
        0.0166,-0.081,-0.0177,
        -0.0112,-0.0905,-0.0177,
        0.0166,-0.081,-0.0177,
        0.017,-0.0814,-0.0177,
        -0.033,-0.0442,-0.0177,
        -0.033,-0.0632,-0.0177,
        -0.033,-0.0442,-0.0161,
        -0.033,-0.0442,-0.0161,
        -0.033,-0.0632,-0.0177,
        -0.033,-0.0632,-0.0161,
        0.022,-0.1032,-0.0161,
        0.022,-0.1032,-0.0177,
        0.022,-0.0905,-0.0161,
        0.022,-0.0905,-0.0161,
        0.022,-0.1032,-0.0177,
        0.022,-0.0905,-0.0177,
        0.022,-0.0905,-0.0161,
        0.022,-0.0905,-0.0177,
        0.022,-0.0632,-0.0161,
        0.022,-0.0632,-0.0161,
        0.022,-0.0905,-0.0177,
        0.022,-0.0632,-0.0177,
        0.022,-0.0632,-0.0177,
        0.034,-0.0632,-0.0177,
        0.022,-0.0632,-0.0161,
        0.022,-0.0632,-0.0161,
        0.034,-0.0632,-0.0177,
        0.034,-0.0632,-0.0161,
        -0.033,-0.0632,-0.0177,
        -0.021,-0.0632,-0.0177,
        -0.033,-0.0632,-0.0161,
        -0.033,-0.0632,-0.0161,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0632,-0.0161,
        -0.021,-0.0632,-0.0161,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0632,-0.0177,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.0905,-0.0161,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.1032,-0.0161,
        -0.021,-0.1032,-0.0161,
        -0.021,-0.0905,-0.0177,
        -0.021,-0.1032,-0.0177,
        0.0005,-0.0717,-0.0177,
        0.0005,-0.0717,-0.0161,
        0.0032,-0.0712,-0.0177,
        0.0032,-0.0712,-0.0177,
        0.0005,-0.0717,-0.0161,
        0.0032,-0.0712,-0.0161,
        0.0032,-0.0712,-0.0177,
        0.0032,-0.0712,-0.0161,
        0.0056,-0.0697,-0.0177,
        0.0056,-0.0697,-0.0177,
        0.0032,-0.0712,-0.0161,
        0.0056,-0.0697,-0.0161,
        0.0056,-0.0697,-0.0177,
        0.0056,-0.0697,-0.0161,
        0.0072,-0.0675,-0.0177,
        0.0072,-0.0675,-0.0177,
        0.0056,-0.0697,-0.0161,
        0.0072,-0.0675,-0.0161,
        0.0072,-0.0675,-0.0177,
        0.0072,-0.0675,-0.0161,
        0.008,-0.0649,-0.0177,
        0.008,-0.0649,-0.0177,
        0.0072,-0.0675,-0.0161,
        0.008,-0.0649,-0.0161,
        0.008,-0.0649,-0.0177,
        0.008,-0.0649,-0.0161,
        0.0077,-0.0621,-0.0177,
        0.0077,-0.0621,-0.0177,
        0.008,-0.0649,-0.0161,
        0.0077,-0.0621,-0.0161,
        0.0077,-0.0621,-0.0177,
        0.0077,-0.0621,-0.0161,
        0.0065,-0.0597,-0.0177,
        0.0065,-0.0597,-0.0177,
        0.0077,-0.0621,-0.0161,
        0.0065,-0.0597,-0.0161,
        0.0065,-0.0597,-0.0177,
        0.0065,-0.0597,-0.0161,
        0.0045,-0.0578,-0.0177,
        0.0045,-0.0578,-0.0177,
        0.0065,-0.0597,-0.0161,
        0.0045,-0.0578,-0.0161,
        0.0045,-0.0578,-0.0177,
        0.0045,-0.0578,-0.0161,
        0.0019,-0.0568,-0.0177,
        0.0019,-0.0568,-0.0177,
        0.0045,-0.0578,-0.0161,
        0.0019,-0.0568,-0.0161,
        0.0019,-0.0568,-0.0177,
        0.0019,-0.0568,-0.0161,
        -0.0009,-0.0568,-0.0177,
        -0.0009,-0.0568,-0.0177,
        0.0019,-0.0568,-0.0161,
        -0.0009,-0.0568,-0.0161,
        -0.0009,-0.0568,-0.0177,
        -0.0009,-0.0568,-0.0161,
        -0.0034,-0.0578,-0.0177,
        -0.0034,-0.0578,-0.0177,
        -0.0009,-0.0568,-0.0161,
        -0.0034,-0.0578,-0.0161,
        -0.0034,-0.0578,-0.0177,
        -0.0034,-0.0578,-0.0161,
        -0.0055,-0.0597,-0.0177,
        -0.0055,-0.0597,-0.0177,
        -0.0034,-0.0578,-0.0161,
        -0.0055,-0.0597,-0.0161,
        -0.0055,-0.0597,-0.0177,
        -0.0055,-0.0597,-0.0161,
        -0.0067,-0.0621,-0.0177,
        -0.0067,-0.0621,-0.0177,
        -0.0055,-0.0597,-0.0161,
        -0.0067,-0.0621,-0.0161,
        -0.0067,-0.0621,-0.0177,
        -0.0067,-0.0621,-0.0161,
        -0.0069,-0.0649,-0.0177,
        -0.0069,-0.0649,-0.0177,
        -0.0067,-0.0621,-0.0161,
        -0.0069,-0.0649,-0.0161,
        -0.0069,-0.0649,-0.0177,
        -0.0069,-0.0649,-0.0161,
        -0.0062,-0.0675,-0.0177,
        -0.0062,-0.0675,-0.0177,
        -0.0069,-0.0649,-0.0161,
        -0.0062,-0.0675,-0.0161,
        -0.0062,-0.0675,-0.0177,
        -0.0062,-0.0675,-0.0161,
        -0.0045,-0.0697,-0.0177,
        -0.0045,-0.0697,-0.0177,
        -0.0062,-0.0675,-0.0161,
        -0.0045,-0.0697,-0.0161,
        -0.0045,-0.0697,-0.0177,
        -0.0045,-0.0697,-0.0161,
        -0.0022,-0.0712,-0.0177,
        -0.0022,-0.0712,-0.0177,
        -0.0045,-0.0697,-0.0161,
        -0.0022,-0.0712,-0.0161,
        -0.0022,-0.0712,-0.0177,
        -0.0022,-0.0712,-0.0161,
        0.0005,-0.0717,-0.0177,
        0.0005,-0.0717,-0.0177,
        -0.0022,-0.0712,-0.0161,
        0.0005,-0.0717,-0.0161,
        -0.033,-0.1032,-0.0177,
        -0.033,-0.1032,-0.0161,
        -0.021,-0.1032,-0.0177,
        -0.021,-0.1032,-0.0177,
        -0.033,-0.1032,-0.0161,
        -0.021,-0.1032,-0.0161,
        0.034,-0.1032,-0.0177,
        0.034,-0.1032,-0.0161,
        0.034,-0.1162,-0.0177,
        0.034,-0.1162,-0.0177,
        0.034,-0.1032,-0.0161,
        0.034,-0.1162,-0.0161,
        0.022,-0.1032,-0.0161,
        0.034,-0.1032,-0.0161,
        0.022,-0.1032,-0.0177,
        0.022,-0.1032,-0.0177,
        0.034,-0.1032,-0.0161,
        0.034,-0.1032,-0.0177,
        -0.033,-0.1162,-0.0161,
        -0.033,-0.1032,-0.0161,
        -0.033,-0.1162,-0.0177,
        -0.033,-0.1162,-0.0177,
        -0.033,-0.1032,-0.0161,
        -0.033,-0.1032,-0.0177,
        0.034,-0.1162,-0.0177,
        0.034,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0177,
        0.034,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0161,
        -0.0112,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0161,
        -0.033,-0.1162,-0.0177,
        -0.033,-0.1162,-0.0177,
        -0.0112,-0.1162,-0.0161,
        -0.033,-0.1162,-0.0161,
        0.034,-0.0442,-0.0177,
        0.0185,-0.0162,-0.0177,
        0.034,-0.0442,-0.0161,
        0.034,-0.0442,-0.0161,
        0.0185,-0.0162,-0.0177,
        0.0185,-0.0162,-0.0161,
        -0.0175,-0.0162,-0.0177,
        -0.033,-0.0442,-0.0177,
        -0.0175,-0.0162,-0.0161,
        -0.0175,-0.0162,-0.0161,
        -0.033,-0.0442,-0.0177,
        -0.033,-0.0442,-0.0161,
        -0.0165,-0.0815,-0.0177,
        -0.0165,-0.0815,-0.0161,
        -0.016,-0.0814,-0.0177,
        -0.016,-0.0814,-0.0177,
        -0.0165,-0.0815,-0.0161,
        -0.016,-0.0814,-0.0161,
        -0.016,-0.0814,-0.0177,
        -0.016,-0.0814,-0.0161,
        -0.0156,-0.081,-0.0177,
        -0.0156,-0.081,-0.0177,
        -0.016,-0.0814,-0.0161,
        -0.0156,-0.081,-0.0161,
        -0.0156,-0.081,-0.0177,
        -0.0156,-0.081,-0.0161,
        -0.0154,-0.0805,-0.0177,
        -0.0154,-0.0805,-0.0177,
        -0.0156,-0.081,-0.0161,
        -0.0154,-0.0805,-0.0161,
        -0.0154,-0.0805,-0.0177,
        -0.0154,-0.0805,-0.0161,
        -0.0156,-0.0799,-0.0177,
        -0.0156,-0.0799,-0.0177,
        -0.0154,-0.0805,-0.0161,
        -0.0156,-0.0799,-0.0161,
        -0.0156,-0.0799,-0.0177,
        -0.0156,-0.0799,-0.0161,
        -0.016,-0.0795,-0.0177,
        -0.016,-0.0795,-0.0177,
        -0.0156,-0.0799,-0.0161,
        -0.016,-0.0795,-0.0161,
        -0.016,-0.0795,-0.0177,
        -0.016,-0.0795,-0.0161,
        -0.0165,-0.0794,-0.0177,
        -0.0165,-0.0794,-0.0177,
        -0.016,-0.0795,-0.0161,
        -0.0165,-0.0794,-0.0161,
        -0.0165,-0.0794,-0.0177,
        -0.0165,-0.0794,-0.0161,
        -0.017,-0.0795,-0.0177,
        -0.017,-0.0795,-0.0177,
        -0.0165,-0.0794,-0.0161,
        -0.017,-0.0795,-0.0161,
        -0.017,-0.0795,-0.0177,
        -0.017,-0.0795,-0.0161,
        -0.0174,-0.0799,-0.0177,
        -0.0174,-0.0799,-0.0177,
        -0.017,-0.0795,-0.0161,
        -0.0174,-0.0799,-0.0161,
        -0.0174,-0.0799,-0.0177,
        -0.0174,-0.0799,-0.0161,
        -0.0175,-0.0805,-0.0177,
        -0.0175,-0.0805,-0.0177,
        -0.0174,-0.0799,-0.0161,
        -0.0175,-0.0805,-0.0161,
        -0.0175,-0.0805,-0.0177,
        -0.0175,-0.0805,-0.0161,
        -0.0174,-0.081,-0.0177,
        -0.0174,-0.081,-0.0177,
        -0.0175,-0.0805,-0.0161,
        -0.0174,-0.081,-0.0161,
        -0.0174,-0.081,-0.0177,
        -0.0174,-0.081,-0.0161,
        -0.017,-0.0814,-0.0177,
        -0.017,-0.0814,-0.0177,
        -0.0174,-0.081,-0.0161,
        -0.017,-0.0814,-0.0161,
        -0.017,-0.0814,-0.0177,
        -0.017,-0.0814,-0.0161,
        -0.0165,-0.0815,-0.0177,
        -0.0165,-0.0815,-0.0177,
        -0.017,-0.0814,-0.0161,
        -0.0165,-0.0815,-0.0161,
        -0.0165,-0.1015,-0.0177,
        -0.0165,-0.1015,-0.0161,
        -0.016,-0.1014,-0.0177,
        -0.016,-0.1014,-0.0177,
        -0.0165,-0.1015,-0.0161,
        -0.016,-0.1014,-0.0161,
        -0.016,-0.1014,-0.0177,
        -0.016,-0.1014,-0.0161,
        -0.0156,-0.101,-0.0177,
        -0.0156,-0.101,-0.0177,
        -0.016,-0.1014,-0.0161,
        -0.0156,-0.101,-0.0161,
        -0.0156,-0.101,-0.0177,
        -0.0156,-0.101,-0.0161,
        -0.0154,-0.1005,-0.0177,
        -0.0154,-0.1005,-0.0177,
        -0.0156,-0.101,-0.0161,
        -0.0154,-0.1005,-0.0161,
        -0.0154,-0.1005,-0.0177,
        -0.0154,-0.1005,-0.0161,
        -0.0156,-0.0999,-0.0177,
        -0.0156,-0.0999,-0.0177,
        -0.0154,-0.1005,-0.0161,
        -0.0156,-0.0999,-0.0161,
        -0.0156,-0.0999,-0.0177,
        -0.0156,-0.0999,-0.0161,
        -0.016,-0.0995,-0.0177,
        -0.016,-0.0995,-0.0177,
        -0.0156,-0.0999,-0.0161,
        -0.016,-0.0995,-0.0161,
        -0.016,-0.0995,-0.0177,
        -0.016,-0.0995,-0.0161,
        -0.0165,-0.0994,-0.0177,
        -0.0165,-0.0994,-0.0177,
        -0.016,-0.0995,-0.0161,
        -0.0165,-0.0994,-0.0161,
        -0.0165,-0.0994,-0.0177,
        -0.0165,-0.0994,-0.0161,
        -0.017,-0.0995,-0.0177,
        -0.017,-0.0995,-0.0177,
        -0.0165,-0.0994,-0.0161,
        -0.017,-0.0995,-0.0161,
        -0.017,-0.0995,-0.0177,
        -0.017,-0.0995,-0.0161,
        -0.0174,-0.0999,-0.0177,
        -0.0174,-0.0999,-0.0177,
        -0.017,-0.0995,-0.0161,
        -0.0174,-0.0999,-0.0161,
        -0.0174,-0.0999,-0.0177,
        -0.0174,-0.0999,-0.0161,
        -0.0175,-0.1005,-0.0177,
        -0.0175,-0.1005,-0.0177,
        -0.0174,-0.0999,-0.0161,
        -0.0175,-0.1005,-0.0161,
        -0.0175,-0.1005,-0.0177,
        -0.0175,-0.1005,-0.0161,
        -0.0174,-0.101,-0.0177,
        -0.0174,-0.101,-0.0177,
        -0.0175,-0.1005,-0.0161,
        -0.0174,-0.101,-0.0161,
        -0.0174,-0.101,-0.0177,
        -0.0174,-0.101,-0.0161,
        -0.017,-0.1014,-0.0177,
        -0.017,-0.1014,-0.0177,
        -0.0174,-0.101,-0.0161,
        -0.017,-0.1014,-0.0161,
        -0.017,-0.1014,-0.0177,
        -0.017,-0.1014,-0.0161,
        -0.0165,-0.1015,-0.0177,
        -0.0165,-0.1015,-0.0177,
        -0.017,-0.1014,-0.0161,
        -0.0165,-0.1015,-0.0161,
        0.0175,-0.1015,-0.0177,
        0.0175,-0.1015,-0.0161,
        0.018,-0.1014,-0.0177,
        0.018,-0.1014,-0.0177,
        0.0175,-0.1015,-0.0161,
        0.018,-0.1014,-0.0161,
        0.018,-0.1014,-0.0177,
        0.018,-0.1014,-0.0161,
        0.0184,-0.101,-0.0177,
        0.0184,-0.101,-0.0177,
        0.018,-0.1014,-0.0161,
        0.0184,-0.101,-0.0161,
        0.0184,-0.101,-0.0177,
        0.0184,-0.101,-0.0161,
        0.0186,-0.1005,-0.0177,
        0.0186,-0.1005,-0.0177,
        0.0184,-0.101,-0.0161,
        0.0186,-0.1005,-0.0161,
        0.0186,-0.1005,-0.0177,
        0.0186,-0.1005,-0.0161,
        0.0184,-0.0999,-0.0177,
        0.0184,-0.0999,-0.0177,
        0.0186,-0.1005,-0.0161,
        0.0184,-0.0999,-0.0161,
        0.0184,-0.0999,-0.0177,
        0.0184,-0.0999,-0.0161,
        0.018,-0.0995,-0.0177,
        0.018,-0.0995,-0.0177,
        0.0184,-0.0999,-0.0161,
        0.018,-0.0995,-0.0161,
        0.018,-0.0995,-0.0177,
        0.018,-0.0995,-0.0161,
        0.0175,-0.0994,-0.0177,
        0.0175,-0.0994,-0.0177,
        0.018,-0.0995,-0.0161,
        0.0175,-0.0994,-0.0161,
        0.0175,-0.0994,-0.0177,
        0.0175,-0.0994,-0.0161,
        0.017,-0.0995,-0.0177,
        0.017,-0.0995,-0.0177,
        0.0175,-0.0994,-0.0161,
        0.017,-0.0995,-0.0161,
        0.017,-0.0995,-0.0177,
        0.017,-0.0995,-0.0161,
        0.0166,-0.0999,-0.0177,
        0.0166,-0.0999,-0.0177,
        0.017,-0.0995,-0.0161,
        0.0166,-0.0999,-0.0161,
        0.0166,-0.0999,-0.0177,
        0.0166,-0.0999,-0.0161,
        0.0165,-0.1005,-0.0177,
        0.0165,-0.1005,-0.0177,
        0.0166,-0.0999,-0.0161,
        0.0165,-0.1005,-0.0161,
        0.0165,-0.1005,-0.0177,
        0.0165,-0.1005,-0.0161,
        0.0166,-0.101,-0.0177,
        0.0166,-0.101,-0.0177,
        0.0165,-0.1005,-0.0161,
        0.0166,-0.101,-0.0161,
        0.0166,-0.101,-0.0177,
        0.0166,-0.101,-0.0161,
        0.017,-0.1014,-0.0177,
        0.017,-0.1014,-0.0177,
        0.0166,-0.101,-0.0161,
        0.017,-0.1014,-0.0161,
        0.017,-0.1014,-0.0177,
        0.017,-0.1014,-0.0161,
        0.0175,-0.1015,-0.0177,
        0.0175,-0.1015,-0.0177,
        0.017,-0.1014,-0.0161,
        0.0175,-0.1015,-0.0161,
        0.0175,-0.0815,-0.0177,
        0.0175,-0.0815,-0.0161,
        0.018,-0.0814,-0.0177,
        0.018,-0.0814,-0.0177,
        0.0175,-0.0815,-0.0161,
        0.018,-0.0814,-0.0161,
        0.018,-0.0814,-0.0177,
        0.018,-0.0814,-0.0161,
        0.0184,-0.081,-0.0177,
        0.0184,-0.081,-0.0177,
        0.018,-0.0814,-0.0161,
        0.0184,-0.081,-0.0161,
        0.0184,-0.081,-0.0177,
        0.0184,-0.081,-0.0161,
        0.0186,-0.0805,-0.0177,
        0.0186,-0.0805,-0.0177,
        0.0184,-0.081,-0.0161,
        0.0186,-0.0805,-0.0161,
        0.0186,-0.0805,-0.0177,
        0.0186,-0.0805,-0.0161,
        0.0184,-0.0799,-0.0177,
        0.0184,-0.0799,-0.0177,
        0.0186,-0.0805,-0.0161,
        0.0184,-0.0799,-0.0161,
        0.0184,-0.0799,-0.0177,
        0.0184,-0.0799,-0.0161,
        0.018,-0.0795,-0.0177,
        0.018,-0.0795,-0.0177,
        0.0184,-0.0799,-0.0161,
        0.018,-0.0795,-0.0161,
        0.018,-0.0795,-0.0177,
        0.018,-0.0795,-0.0161,
        0.0175,-0.0794,-0.0177,
        0.0175,-0.0794,-0.0177,
        0.018,-0.0795,-0.0161,
        0.0175,-0.0794,-0.0161,
        0.0175,-0.0794,-0.0177,
        0.0175,-0.0794,-0.0161,
        0.017,-0.0795,-0.0177,
        0.017,-0.0795,-0.0177,
        0.0175,-0.0794,-0.0161,
        0.017,-0.0795,-0.0161,
        0.017,-0.0795,-0.0177,
        0.017,-0.0795,-0.0161,
        0.0166,-0.0799,-0.0177,
        0.0166,-0.0799,-0.0177,
        0.017,-0.0795,-0.0161,
        0.0166,-0.0799,-0.0161,
        0.0166,-0.0799,-0.0177,
        0.0166,-0.0799,-0.0161,
        0.0165,-0.0805,-0.0177,
        0.0165,-0.0805,-0.0177,
        0.0166,-0.0799,-0.0161,
        0.0165,-0.0805,-0.0161,
        0.0165,-0.0805,-0.0177,
        0.0165,-0.0805,-0.0161,
        0.0166,-0.081,-0.0177,
        0.0166,-0.081,-0.0177,
        0.0165,-0.0805,-0.0161,
        0.0166,-0.081,-0.0161,
        0.0166,-0.081,-0.0177,
        0.0166,-0.081,-0.0161,
        0.017,-0.0814,-0.0177,
        0.017,-0.0814,-0.0177,
        0.0166,-0.081,-0.0161,
        0.017,-0.0814,-0.0161,
        0.017,-0.0814,-0.0177,
        0.017,-0.0814,-0.0161,
        0.0175,-0.0815,-0.0177,
        0.0175,-0.0815,-0.0177,
        0.017,-0.0814,-0.0161,
        0.0175,-0.0815,-0.0161
        };

        noVertices = sizeof(vertices) / sizeof(float) / 3;

        std::vector<unsigned int> indices(noVertices);

        for (unsigned int i = 0; i < noVertices; i++) {
            indices[i] = i;
        }

        meshes.push_back(Mesh(Vertex::genList(vertices, noVertices), indices));
    }

    void setPose(glm::vec3 position, b2Rot rotation) {
        this->position = position;
        this->rotation = b2Rot_GetAngle(rotation);
    }
    
    void render(Shader shader, bool setModel = false) {
        glm::mat4 model = glm::mat4(1.0f);

        //Set position
        model = glm::translate(model, glm::vec3(position.x -0.0005f , position.y + MICRAS_HALFHEIGHT*1.325f, position.z));

        // Rotate
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        // Scale
        model = glm::scale(model, size);


        shader.setMat4("model", model);

        Model::render(shader, setModel);
    }
    
};

} // micrasverse::render

#endif // RECTANGLE_HPP
