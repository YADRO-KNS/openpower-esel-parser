/**
 * @brief Parser settings interface.
 *
 * Copyright (c) 2019 YADRO
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

namespace eSEL
{

/**
 * @brief Set Hostboot's symbols file location.
 *
 * @param[in] hbicore - path to Hostboot's core symbols file (hbicore.syms)
 */
void setHostbootSymbols(const char* symbolsFile);

/**
 * @brief Set Hostboot's string file location used by FSP tracer.
 *
 * @param[in] stringFile - path to Hostboot's string file (hbotStringFile)
 */
void setHostbootStrings(const char* stringFile);

/**
 * @brief Set OCC's string file location used by FSP tracer.
 *
 * @param[in] stringFile - path to OCC's string file (occStringFile)
 */
void setOccStrings(const char* stringFile);

/**
 * @brief Set path to FSP trace utility (fsp-trace).
 *
 * @param[in] path - path to the trace utility
 */
void setFspTrace(const char* path);

} // namespace eSEL
