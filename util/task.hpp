/**
 * @brief High level functionality of the console utility.
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

#include "printer.hpp"

#include <vector>

/**
 * @class Task
 * @brief High level functionality of the console utility.
 */
class Task
{
  public:
    /**
     * @brief Constructor.
     *
     * @param[in] printer - printer instance
     */
    Task(const Printer& printer);

    /**
     * @brief Set task: Read, parse and print eSEL from file.
     *
     * @param[in] path - path to the file
     */
    void fromFile(const char* path);

    /**
     * @brief Set task: Read, parse and print eSEL from BMC event.
     *
     * @param[in] id - BMC event ID
     */
    void fromBmcEvent(size_t eventId);

    /**
     * @brief Set task: Read, parse and print eSEL from HBEL partition of PNOR.
     *
     * @param[in] id - eSEL event ID
     */
    void fromPnorEvent(size_t eventId);

    /**
     * @brief Set task: Print list of events of BMC that contains eSEL.
     */
    void listBmcEvent();

    /**
     * @brief Set task: Print list of events from HBEL partition of PNOR.
     */
    void listPnorEvent();

    /**
     * @brief Set ECC handling flag.
     *        If set, ECC bytes will be cut out from eSEL source.
     *
     * @param[in] eccExist - flag indicated that source data has ECC
     */
    void sourceWithEcc(bool eccExist);

    /**
     * @brief Switch to using dump of HBEL partition from file.
     *        By default, data is read from PNOR flash via pflash utility.
     *
     * @param[in] path - path to the dump of HBEL partition
     */
    void pnorHbel(const char* path);

    /**
     * @brief Execute action.
     *
     * @return exit code (0 if success)
     */
    int execute();

  private:
    /**
     * @brief Parse and print eSEL event.
     */
    void printEvent() const;

    /**
     * @brief Read binary file.
     *
     * @param[in] path - path to the file
     *
     * @return file content
     */
    std::vector<uint8_t> readFile(const char* path) const;

    /**
     * @brief Remove ECC (every 9th byte).
     *
     * @param[in] data - original data
     *
     * @return data without ECC
     */
    std::vector<uint8_t> removeEcc(const std::vector<uint8_t>& data) const;

    /**
     * @brief Print list of events of BMC that contains eSEL.
     */
    void printBmcEventList() const;

    /**
     * @brief Read raw event data from BMC event.
     *
     * @return unparsed raw data
     */
    std::vector<uint8_t> readBmcEvent() const;

    /**
     * @brief Read HBEL partition on PNOR.
     *
     * @return HBEL data
     */
    std::vector<uint8_t> readHbel() const;

    /**
     * @brief Print list of events from HBEL partition of PNOR.
     */
    void printPnorEventList() const;

    /**
     * @brief Read raw event data from PNOR.
     *
     * @return unparsed raw data
     */
    std::vector<uint8_t> readPnorEvent() const;

  private:
    /**
     * @enum GeneralAction
     * @brief General action types.
     */
    enum GeneralAction
    {
        PrintSEL,
        PrintBmcList,
        PrintPnorList,
    };
    /** @brief General action type. */
    GeneralAction action_;

    /** @brief Printer instance. */
    const Printer& printer_;
    /** @brief Path to eSEL file. */
    const char* pelFile_;
    /** @brief Event Id to read eSEL from BMC. */
    size_t bmcEventId_;
    /** @brief Event Id to read eSEL from PNOR. */
    size_t pnorEventId_;
    /** @brief Flag: ECC in source data. */
    bool eccExist_;
    /** @brief Path to HBEL dump file. */
    const char* hbelFile_;
};
